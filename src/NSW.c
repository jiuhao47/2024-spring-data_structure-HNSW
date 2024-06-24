#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../inc/definition.h"
#include "../inc/structures.h"

// 初始化HNSW
void InitalGraph(HNSW_Graph *G)
{
    G = (HNSW_Graph *)malloc(sizeof(HNSW_Graph));
    G->highestLevel = -1;                                              // 此时即使第0层也没有点
    G->nodeCount = 0;                                                 // 没有点
    G->pEntryPointList = (Node **)malloc(sizeof(Node *) * (MAX_LAYER + 1)); // 入口点数组
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
    SL->candidatePointList = calloc(2 * MAX_NEAR, sizeof(Node *)); // 候选节点数组 定长
    SL->candidatePointList2 = calloc(2 * MAX_NEAR, sizeof(Node *)); // 候选节点数组的影子数组 定长
    SL->visitedPointList = calloc(1, sizeof(Node *)); // 访问过节点数组 变长 当空间不足时长度加倍
    SL->candidatePointList[0] = G->pEntryPointList[level]; // 第一个候选节点为入口点
    
    //每一层插入节点
    for (int i = level; i >= 0; i--)
    {
        // 给每一层的点分配空间（i不等于level时）
        // 连接上一个节点，然后更新lastNode
        if(i !=level){
            newNode = (Node *)malloc(sizeof(Node));
            newNode->data = data;
            newNode->pList = (Node **)malloc(sizeof(Node *) * MAX_NEAR);
            lastNode->nextLevel = newNode;
            // 第一个候选节点为上一层搜索到的最近节点，其余为NULL
            for(int j = 0; j < 2 * MAX_NEAR; j++){
                SL->candidatePointList[j] = NULL;
                SL->candidatePointList2[j] = NULL;
            }
            SL->candidatePointList[0] = SL->candidatePointList[0]->nextLevel;
            // 释放上一层访问过节点数组 重新分配
            free(SL->visitedPointList);
            SL->visitedPointList = calloc(1, sizeof(Node *));
        }
        if(i == 0){
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
    int sum; // 记录当前待比较的节点数目
    Node *p = SL->candidatePointList[0]; // 第一个候选节点
    for(int i = 0; i < MAX_NEAR; i++){
        SL->candidatePointList[i + 1] = p->pList[i];
    }
    // 计算当前待比较的节点数目（因为plist并不一定有MAX_NEAR个节点，若图在本层的总节点数过少）
    // 同时完成对距离的计算
    for(sum = 0; sum < 2 * MAX_NEAR && SL->candidatePointList[sum] != NULL; sum++)
        SL->candidatePointList[sum]->distance = Distance(newNode, SL->candidatePointList[sum]);
    // 对候选节点进行排序














    // 对候选节点进行排序，然后与影子数组作比较，若前MAX_NEAR个节点均相同，说明已经找到了m-邻近节点，连接即可

    // 若存在不同的节点，更新影子数组，然后继续搜索


    // 在第level层搜索m-邻近节点并进行连接


    // 在第i层搜索m-邻近节点并进行连接
}

void insertionSort(SearchList *SL, int n) {
    // 将候选节点数组中的前n个节点按照距离从小到大排序
    for (int i = 1; i < n; i++) {
        Node *temp = SL->candidatePointList[i];
        int j = i - 1;
        while (j >= 0 && SL->candidatePointList[j]->distance > temp->distance) {
            SL->candidatePointList[j + 1] = SL->candidatePointList[j];
            j--;
        }
        SL->candidatePointList[j + 1] = temp;
    }
}




// 搜索m-邻近节点
void Search(HNSW_Graph *G)
{
    //critical and hard!
}

// 随机插入层数
int RandomLevel()
{
    double u = (double)rand() / RAND_MAX;
    int level = (int)(-log(u) / log(2.0) * RANDOMWeight);
    if(level > MAX_LAYER){
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
