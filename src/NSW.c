#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <sys/types.h>

#include "../inc/definition.h"
#include "../inc/structures.h"

// 初始化HNSW
void InitalGraph(HNSW_Graph **G)
{
    (*G) = (HNSW_Graph *)malloc(sizeof(HNSW_Graph));
    (*G)->highestLevel = -1;                                                   // 事实上此时即使第0层也没有点
    (*G)->nodeCount = 0;                                                       // 没有点
    (*G)->pEntryPointList = (Node **)malloc(sizeof(Node *) * (MAX_LAYER + 1)); // 入口点数组
}

void DeleteGraph(HNSW_Graph **G, Node *nodeList[])
{
    Node *plast, *p;
    // 释放每一个节点在每一层上的空间和连接
    for (int i = 0; i < NODESUM; i++)
    {
        plast = nodeList[i];
        // free
        while (plast != NULL)
        {
            p = plast->nextLevel;
            free(plast->pList);
            free(plast);
            plast = p;
        }
    }
    // 释放入口点数组
    free((*G)->pEntryPointList);
    // 释放HNSW_Graph
    free(*G);
}

// 插入新节点
void InsertNode(HNSW_Graph *G, NodeDataType data, char *filepath_a, char *filepath_b, Node *nodelist[])
{
    int level = RandomLevel();
    Node *lastNode = NULL;       // 记录上一层的同节点，用于连接层与层
    if (level > G->highestLevel) // 每一层的第一个节点都作为入口点
    {
        G->highestLevel = level;
        for (; level >= 0; level--)
        {
            if (G->pEntryPointList[level] == NULL)
            {
                Node *newNode = (Node *)malloc(sizeof(Node));      // 最顶层节点分配空间
                newNode->data = data;                              // 赋值
                newNode->pList = calloc(MAX_NEAR, sizeof(Node *)); // 分配m-邻近节点空间
                newNode->connectCount = 0;                         // 连接数为0
                G->pEntryPointList[level] = newNode;
                nodelist[data] = newNode; // 保存节点
                if (lastNode != NULL)
                {
                    lastNode->nextLevel = newNode;
                }
                lastNode = newNode;
            }
            else
            {
                break;
            }
        }
    }
    Node *newNode = (Node *)malloc(sizeof(Node));      // 最顶层节点分配空间
    newNode->data = data;                              // 赋值
    newNode->pList = calloc(MAX_NEAR, sizeof(Node *)); // 分配m-邻近节点空间
    newNode->connectCount = 0;
    if (nodelist[data] == NULL) // 保存节点
    {
        nodelist[data] = newNode;
    }
    if (lastNode != NULL)
    {
        lastNode->nextLevel = newNode; // 连接数为0
    }
    // 初始化搜索列表
    SearchList *SL = (SearchList *)malloc(sizeof(SearchList));
    SL->candidatePointList = calloc(NODESUM, sizeof(Node *));                      // 候选节点数组 定长
    SL->candidatePointList2 = calloc(NODESUM, sizeof(Node *));                     // 候选节点数组的影子数组 定长
    SL->visitedPointList = calloc(1, sizeof(Node *));                              // 访问过节点数组 变长 当空间不足时长度加倍
    SL->visitedPointCount = 0;                                                     // 访问过节点数
    SL->candidatePointCount = 0;                                                   // 候选节点数
    SL->candidatePointCount2 = 0;                                                  // 候选节点数s
    SL->candidatePointList[SL->candidatePointCount++] = G->pEntryPointList[level]; // 第一个候选节点为入口点

    // 每一层插入节点
    for (int i = level; i >= 0; i--)
    {
        // 给每一层的点分配空间（i不等于level时）
        // 连接上一个节点，然后更新lastNode
        if (i != level)
        {
            newNode = (Node *)malloc(sizeof(Node));
            newNode->data = data;
            newNode->pList = calloc(MAX_NEAR, sizeof(Node *));
            newNode->connectCount = 0;
            lastNode->nextLevel = newNode;
            // 第一个候选节点为上一层搜索到的最近节点，其余为NULL
            for (int j = 1; j < NODESUM; j++)
            {
                SL->candidatePointList[j] = NULL;
                SL->candidatePointList2[j] = NULL;
            }
            SL->candidatePointList2[0] = NULL;
            SL->candidatePointList[0] = SL->candidatePointList[0]->nextLevel;
            SL->candidatePointCount = 1;
            SL->candidatePointCount2 = 0;
            // 释放上一层访问过节点数组 重新分配
            free(SL->visitedPointList);
            SL->visitedPointCount = 0;
            SL->visitedPointList = NULL;
            SL->visitedPointList = calloc(1, sizeof(Node *));
        }
        if (i == 0)
        {
            newNode->nextLevel = NULL;
        }
        // 每一层要连接m-邻近节点，上面先维护更新SL的成员，再调用FindNode函数
        FindNode(G, newNode, i, SL, filepath_a, filepath_b);
        // 连接
        for (int j = 0; (j < MAX_NEAR) && (SL->candidatePointList[j] != NULL); j++)
        {
            newNode->pList[newNode->connectCount] = SL->candidatePointList[j];
            newNode->connectCount++;
            //  候选点的初始分配空间为MAX_NEAR，若不足则重新分配+1空间，然后插入新节点
            if (SL->candidatePointList[j]->connectCount >= MAX_NEAR) // 初始分配空间已满
            {
                SL->candidatePointList[j]->pList = realloc(SL->candidatePointList[j]->pList, sizeof(Node *) * (SL->candidatePointList[j]->connectCount + 1));
            }
            SL->candidatePointList[j]->pList[SL->candidatePointList[j]->connectCount++] = newNode;
        }
        // 注意第0层连接NULL
        lastNode = newNode;
    }
    G->nodeCount++;

    // 释放SL
    free(SL->candidatePointList);
    free(SL->candidatePointList2);
    free(SL->visitedPointList);
    free(SL);
}

// 连接新节点与第level层的m-邻近节点
void FindNode(HNSW_Graph *G, Node *newNode, int level, SearchList *SL, char *filepath_a, char *filepath_b)
{
    // 第一步，将入口点及其相连节点存入候选节点数组
    Node *p = SL->candidatePointList[0]; // 第一个候选节点
    SL->candidatePointCount = 1;
    InsertVisitedPointList(SL, p); // 将第一个候选节点存入访问过节点数组
    for (int i = 0; (i < p->connectCount) && (p->pList[i] != NULL); i++)
    {
        SL->candidatePointList[i + 1] = p->pList[i];
        InsertVisitedPointList(SL, p->pList[i]);
        SL->candidatePointCount++;
    }
    // 完成对距离的计算 复制候选节点数组到影子数组
    for (int i = 0; i < SL->candidatePointCount; i++)
    {
        SL->candidatePointList[i]->distance = Distance(newNode, SL->candidatePointList[i], filepath_a, filepath_b);
    }
    // 对候选节点进行排序
    insertionSort(SL, SL->candidatePointCount);
    // 复制候选节点数组到影子数组
    for (int i = 0; i < SL->candidatePointCount; i++)
    {
        SL->candidatePointList2[i] = SL->candidatePointList[i];
    }
    SL->candidatePointCount2 = SL->candidatePointCount;
    // 接下来一直循环，直到找到m-邻近节点
    // int count = 0;
    while (1)
    {
        // count++;
        /*
        for (int i = 0; i < SL->candidatePointCount; i++)
        {
            printf("A: candidatePointList[%d]=%d\n", i, SL->candidatePointList[i]->data);
        }
        for (int i = 0; i < SL->candidatePointCount2 && SL->candidatePointList2[i] != NULL; i++)
        {
            printf("A: candidatePointList2[%d]=%d\n", i, SL->candidatePointList2[i]->data);
        }
        */
        // 从（影子的）第一个候选节点开始搜索m-邻近节点，并存入候选节点数组
        for (int i = 0; i <= MAX_NEAR && SL->candidatePointList2[i] != NULL; i++)
        {
            for (int j = 0; j < SL->candidatePointList2[i]->connectCount && SL->candidatePointList2[i]->pList[j] != NULL; j++)
            {
                InsertCandidatePointList(SL, SL->candidatePointList2[i]->pList[j], newNode, filepath_a, filepath_b);
                InsertVisitedPointList(SL, SL->candidatePointList2[i]->pList[j]);
            }
        }
        /*
        for (int i = 0; i < SL->candidatePointCount; i++)
        {
            printf("B: candidatePointList[%d]=%d\n", i, SL->candidatePointList[i]->data);
        }
        for (int i = 0; i < SL->candidatePointCount2 && SL->candidatePointList2[i] != NULL; i++)
        {
            printf("B: candidatePointList2[%d]=%d\n", i, SL->candidatePointList2[i]->data);
        }
        */
        // 判断候选节点数组和影子数组（至少）前MAX_NEAR项是否相同，若不相同则更新影子数组，若相同则找到m-邻近节点,进行连接
        // for (int i = 0; i < MAX_NEAR && SL->candidatePointList[i] != NULL; i++)
        // XXX: 列表动态判断相等循环条件存疑
        for (int i = 0; i < MAX_NEAR; i++)
        {
            if (SL->candidatePointList[i] != SL->candidatePointList2[i])
            {
                // printf("Enter\n");
                for (int j = i; j < SL->candidatePointCount; j++)
                {
                    SL->candidatePointList2[j] = SL->candidatePointList[j];
                }
                break;
            }
            if (i == MAX_NEAR - 1)
            {
                // printf("Count=%d\n", count);
                return;
            }
        }
    }
}

// 将候选节点数组中的前n个节点按照距离做插入排序
void insertionSort(SearchList *SL, int n)
{
    for (int i = 1; i < n; i++)
    {
        Node *temp = SL->candidatePointList[i];
        int j = i - 1;
        while (j >= 0 && SL->candidatePointList[j]->distance < temp->distance)
        {
            SL->candidatePointList[j + 1] = SL->candidatePointList[j];
            j--;
        }
        SL->candidatePointList[j + 1] = temp;
    }
}

// 将节点存入访问过节点数组
void InsertVisitedPointList(SearchList *SL, Node *node)
{

    // 若空间不足（即Count为2的幂方时），空间翻倍，然后初始化
    if (SL->visitedPointCount && !(SL->visitedPointCount & (SL->visitedPointCount - 1)))
    {
        SL->visitedPointList = realloc(SL->visitedPointList, sizeof(Node *) * SL->visitedPointCount * 2);

        for (int i = SL->visitedPointCount; i < SL->visitedPointCount * 2; i++)
        {
            SL->visitedPointList[i] = NULL;
        }
    }
    SL->visitedPointList[SL->visitedPointCount++] = node;
}

// 将节点存入候选节点数组，且边插入边排序，可利用二分查找优化
void InsertCandidatePointList(SearchList *SL, Node *node, Node *newNode, char *filepath_a, char *filepath_b)
{
    // 查询节点是否已经在候选节点数组中
    for (int i = 0; i < SL->visitedPointCount; i++)
    {
        if (SL->visitedPointList[i] == node)
        {
            return;
        }
    }
    node->distance = Distance(newNode, node, filepath_a, filepath_b);
    // 二分查找，插入候选节点数组，后面元素后移
    int left = 0;
    int right = SL->candidatePointCount - 1;
    int mid;
    while (left <= right)
    {
        mid = (left + right) / 2;
        if (SL->candidatePointList[mid]->distance == node->distance)
        {
            break;
        }
        else if (SL->candidatePointList[mid]->distance > node->distance)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    for (int i = SL->candidatePointCount; i > left; i--)
    {
        // 若SL->candidatePointCount == NODESUM, 则越界
        if (i == NODESUM)
        {
            continue;
        }
        SL->candidatePointList[i] = SL->candidatePointList[i - 1];
    }
    SL->candidatePointList[left] = node;
    if (SL->candidatePointCount < NODESUM)
    {
        SL->candidatePointCount++;
    }
}

// 搜索m-邻近节点
Node **Search(HNSW_Graph *G, NodeDataType data, char *filepath_a, char *filepath_b)
{
    Node **result = (Node **)malloc(sizeof(Node *) * SEARCH_NUM);
    // 初始化待搜索节点
    Node *SearchNode = (Node *)malloc(sizeof(Node));
    SearchNode->data = data;
    Node *p = G->pEntryPointList[G->highestLevel]; // 从最高层开始搜索

    // 初始化搜索列表
    SearchList *SL = (SearchList *)malloc(sizeof(SearchList));
    SL->candidatePointList = calloc(NODESUM, sizeof(Node *));  // 候选节点数组 定长
    SL->candidatePointList2 = calloc(NODESUM, sizeof(Node *)); // 候选节点数组的影子数组 定长
    SL->visitedPointList = calloc(1, sizeof(Node *));          // 访问过节点数组 变长 当空间不足时长度加倍
    SL->visitedPointCount = 0;                                 // 访问过节点数
    SL->candidatePointCount = 0;                               // 候选节点数
    SL->candidatePointList[SL->candidatePointCount++] = p;     // 第一个候选节点为入口点

    // 在每一层进行搜索，进入下一层时将上一层的最近节点作为新的入口
    for (int i = G->highestLevel; i >= 0; i--)
    {
        if (i != G->highestLevel)
        {
            p = p->nextLevel;
            for (int j = 0; j < NODESUM; j++)
            {
                SL->candidatePointList[j] = NULL;
                SL->candidatePointList2[j] = NULL;
            }
            SL->candidatePointList[0] = p;
            free(SL->visitedPointList);
            SL->visitedPointList = NULL;
            SL->visitedPointCount = 0;
            SL->visitedPointList = calloc(1, sizeof(Node *));
        }
        FindNode(G, SearchNode, i, SL, filepath_a, filepath_b);
        p = SL->candidatePointList[0];
        if (i == 0)
        {
            for (int j = 0; j < SEARCH_NUM && SEARCH_NUM <= SL->candidatePointCount; j++)
            {
                result[j] = SL->candidatePointList[j];
            }
        }
    }
    // 释放空间
    free(SL->candidatePointList);
    free(SL->candidatePointList2);
    free(SL->visitedPointList);
    free(SL);
    return result;
}

// 随机插入层数
int RandomLevel()
{
    double u = (double)rand() / RAND_MAX;
    int level = (int)(-log(u) / log(2.0) * RANDOMWeight);
    if (level > MAX_LAYER)
    {
        level = MAX_LAYER;
    }
    return level;
}

float CosineDistance(float *a, float *b, int size)
{
    float dot = 0.0, denom_a = 0.0, denom_b = 0.0;
    for (int i = 0; i < size; ++i)
    {
        dot += a[i] * b[i];
        denom_a += a[i] * a[i];
        denom_b += b[i] * b[i];
    }
    return dot / (sqrt(denom_a) * sqrt(denom_b));
}

void ReadVectorFromFile(char *filename, float *vector, int size)
{
    FILE *file = fopen(filename, "r");
    for (int i = 0; i < size; ++i)
    {
        fscanf(file, "%f", &vector[i]);
    }
    fclose(file);
}

// 计算两个节点之间的距离（当前为余弦距离函数）

float Distance(Node *a, Node *b, char *filepath_a, char *filepath_b)
{
    float distance;
    char filepath1[FILEPATHLEN];
    char filepath2[FILEPATHLEN];
    sprintf(filepath1, filepath_a, a->data);
    sprintf(filepath2, filepath_b, b->data);
    float vector1[VECTORSIZE];
    float vector2[VECTORSIZE];
    ReadVectorFromFile(filepath1, vector1, VECTORSIZE);
    ReadVectorFromFile(filepath2, vector2, VECTORSIZE);
    distance = CosineDistance(vector1, vector2, VECTORSIZE);
    return distance;
}

// 暴力搜索

int findMinIndex(float *result)
{
    int minIndex = 0;
    for (int i = 0; i < SEARCH_NUM; i++)
    {
        if (result[i] < result[minIndex])
        {
            minIndex = i;
        }
    }
    return minIndex;
}

void BruteForceSearch(Node *a, float *result, int *resultIndex, char *filepath_a, char *filepath_b)
{
    Node temp;
    for (int i = 0; i < NODESUM; i++)
    {
        temp.data = i;
        float distance = Distance(a, &temp, filepath_a, filepath_b);
        int minIndex = 0;
        minIndex = findMinIndex(result);
        if (distance > result[minIndex])
        {
            result[minIndex] = distance;
            resultIndex[minIndex] = i;
        }
    }
}

void ShowMenu(int choice, char *code)
{
    switch (choice)
    {
    case 0:
    {
        // system("clear");
        printf("---------------HNSW---------------\n");
        printf("0. Exit\n");
        printf("1. HNSW: Build Graph\n");
        printf("2. HNSW: Search\n");

        printf("Please input your choice:");
        break;
    }
    case 1:
    {
        // system("clear");
        printf("-----------HNSW-DATASET-----------\n");
        printf("0. Return\n");
        printf("1. All\n");
        printf("2. Aircraft\n");
        printf("3. Clothing\n");
        printf("4. Flower\n");
        printf("5. food\n");
        printf("6. Logo\n");
        printf("7. Custom\n");
        printf("Please input your choice:");
        break;
    }
    case 2:
    {
        // system("clear");
        printf("--------HNSW-SEARCH-OBJECT--------\n");
        printf("0. Return\n");
        printf("1. All\n");
        printf("2. Aircraft\n");
        printf("3. Clothing\n");
        printf("4. Flower\n");
        printf("5. food\n");
        printf("6. Logo\n");
        printf("7. Custom\n");
        printf("Please input your choice:");
        break;
    }
    case 3:
    {
        printf("\nFiles:\n");
        system(code);
        printf("Please input the index:");
    }
    }
}
