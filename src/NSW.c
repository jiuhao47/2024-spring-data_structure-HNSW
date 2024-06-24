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
    (*G)->highestLevel = 0;                                              // 事实上此时即使第0层也没有点
    (*G)->nodeCount = 0;                                                 // 没有点
    (*G)->pEntryPointList = (Node **)malloc(sizeof(Node *) * MAX_LAYER); // 入口点数组
}

// 插入新节点

Node *InsertNode(HNSW_Graph *G, NodeDataType data)
{
    int level = RandomLevel();
    Node *lastNode;                                              // 记录上一层的同节点，用于连接层与层
    Node *newNode = (Node *)malloc(sizeof(Node));                // 最顶层节点分配空间
    newNode->data = data;                                        // 赋值
    newNode->pList = (Node **)malloc(sizeof(Node *) * MAX_NEAR); // 分配m-邻近节点空间
    if (level > G->highestLevel)                                 // 每一层的第一个节点都作为入口点
    {
        G->highestLevel = level;
        G->pEntryPointList[level] = newNode;
    }

    // 初始化搜索列表
    SearchList *SL = (SearchList *)malloc(sizeof(SearchList));
    SL->candidatePointList = calloc(MAX_POINT, sizeof(Node *));                    // 候选节点数组 定长
    SL->candidatePointList2 = calloc(MAX_POINT, sizeof(Node *));                   // 候选节点数组的影子数组 定长
    SL->visitedPointList = calloc(1, sizeof(Node *));                              // 访问过节点数组 变长 当空间不足时长度加倍
    SL->visitedPointCount = 0;                                                     // 访问过节点数
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
            newNode->pList = (Node **)malloc(sizeof(Node *) * MAX_NEAR);
            lastNode->nextLevel = newNode;
            // 第一个候选节点为上一层搜索到的最近节点，其余为NULL
            for (int j = 0; j < MAX_POINT; j++)
            {
                SL->candidatePointList[j] = NULL;
                SL->candidatePointList2[j] = NULL;
            }
            SL->candidatePointList[0] = SL->candidatePointList[0]->nextLevel;
            // 释放上一层访问过节点数组 重新分配
            free(SL->visitedPointList);
            SL->visitedPointList = calloc(1, sizeof(Node *));
        }
        if (i == 0)
        {
            newNode->nextLevel = NULL;
        }
        // 每一层要连接m-邻近节点，上面先维护更新SL的成员，再调用ConnectNode函数
        ConnectNode(G, newNode, i, SL);
        // 注意第0层连接NULL
        lastNode = newNode;
    }
    return newNode;
}

// 连接新节点与第level层的m-邻近节点
void ConnectNode(HNSW_Graph *G, Node *newNode, int level, SearchList *SL)
{
    // 第一步，将入口点及其相连节点存入候选节点数组
    int sum;                             // 记录当前待比较的节点数目
    Node *p = SL->candidatePointList[0]; // 第一个候选节点
    InsertVisitedPointList(SL, p);       // 将第一个候选节点存入访问过节点数组
    for (int i = 0; i < MAX_NEAR && p->pList[i] != NULL; i++)
    {
        SL->candidatePointList[i + 1] = p->pList[i];
        InsertVisitedPointList(SL, p->pList[i]);
        SL->candidatePointCount++;
    }
    // 完成对距离的计算 复制候选节点数组到影子数组
    for (int i = 0; i < SL->candidatePointCount; i++)
    {
        SL->candidatePointList[i]->distance = Distance(newNode, SL->candidatePointList[i]);
    }
    // 对候选节点进行排序
    insertionSort(SL, SL->candidatePointCount);
    // 复制候选节点数组到影子数组
    for (int i = 0; i < SL->candidatePointCount; i++)
    {
        SL->candidatePointList2[i] = SL->candidatePointList[i];
    }

    // 第二步，从第一个候选节点开始搜索m-邻近节点，并存入候选节点数组

    // 判断候选节点数组和影子数组是否相同，若不相同则更新影子数组，若相同则找到m-邻近节点

    // 从排序后的候选节点中的第一个

    // 对候选节点进行排序，然后与影子数组作比较，若前MAX_NEAR个节点均相同，说明已经找到了m-邻近节点，连接即可

    // 若存在不同的节点，更新影子数组，然后继续搜索

    // 在第level层搜索m-邻近节点并进行连接

    // 在第i层搜索m-邻近节点并进行连接
}

// 将候选节点数组中的前n个节点按照距离做插入排序
void insertionSort(SearchList *SL, int n)
{
    for (int i = 1; i < n; i++)
    {
        Node *temp = SL->candidatePointList[i];
        int j = i - 1;
        while (j >= 0 && SL->candidatePointList[j]->distance > temp->distance)
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
void InsertCandidatePointList(SearchList *SL, Node *node)
{
    // 查询节点是否已经在候选节点数组中
    for (int i = 0; i < SL->visitedPointCount; i++)
    {
        if (SL->visitedPointList[i] == node)
        {
            return;
        }
    }
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
        else if (SL->candidatePointList[mid]->distance < node->distance)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    for (int i = SL->candidatePointCount; i > mid; i--)
    {
        // 若SL->candidatePointCount == MAX_POINT，则会越界
        if (i == MAX_POINT)
        {
            continue;
        }
        SL->candidatePointList[i] = SL->candidatePointList[i - 1];
    }
    SL->candidatePointList[mid] = node;
    if (SL->candidatePointCount < MAX_POINT)
    {
        SL->candidatePointCount++;
    }
}

// 搜索m-邻近节点
void Search(HNSW_Graph *G)
{
    // critical and hard!
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

float Distance(Node *a, Node *b)
{
    float distance;
    char filepath1[FILEPATHLEN];
    char filepath2[FILEPATHLEN];
    sprintf(filepath1, FILEPATH, a->data);
    sprintf(filepath2, FILEPATH, b->data);
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
    for (int i = 0; i < MAX_NEAR; i++)
    {
        if (result[i] < result[minIndex])
        {
            minIndex = i;
        }
    }
    return minIndex;
}

void BruteForceSearch(Node *a, float *result, int *resultIndex)
{
    Node temp;
    for (int i = 0; i < DATASUM; i++)
    {
        temp.data = i;
        float distance = Distance(a, &temp);
        // printf("%f\n", distance);
        int minIndex = findMinIndex(result);
        if (distance > result[minIndex])
        {
            result[minIndex] = distance;
            resultIndex[minIndex] = i;
        }
    }
}

void ShowMenu(int choice)
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
        printf("1. Custom\n");
        printf("2-%d: Given images from random selecting\n", SEARCH_OBJECT_NUM + 2);
        printf("Please input your choice:");
        break;
    }
    }
}
