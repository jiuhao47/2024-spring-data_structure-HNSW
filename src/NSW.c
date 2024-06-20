#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../inc/definition.h"
#include "../inc/structures.h"

// 初始化HNSW
void InitalGraph(HNSW_Graph *G)
{
    G = (HNSW_Graph *)malloc(sizeof(HNSW_Graph));
    G->highestLevel = 0;                                              // 事实上此时即使第0层也没有点
    G->nodeCount = 0;                                                 // 没有点
    G->pEntryPointList = (Node **)malloc(sizeof(Node *) * MAX_LAYER); // 入口点数组
}

// 插入新节点
Node *InsertNode(HNSW_Graph *G, NodeDataType data)
{
    int level = RandomLevel();
    Node *lastNode;                                              // 记录上一个节点，用于连接
    Node *newNode = (Node *)malloc(sizeof(Node));                // 最顶层节点分配空间
    newNode->data = data;                                        // 赋值
    newNode->pList = (Node **)malloc(sizeof(Node *) * MAX_NEAR); // 分配m-邻近节点空间
    if (level > G->highestLevel)                                 // 每一层的第一个节点都作为入口点
    {
        G->highestLevel = level;
        G->pEntryPointList[level] = newNode;
    }
    for (int i = level; i >= 0; i--)
    {
        // 给每一层的点分配空间（i不等于level时）
        // 连接上一个节点，然后更新lastNode
        if(i !=level){
            newNode = (Node *)malloc(sizeof(Node));
            newNode->data = data;
            newNode->pList = (Node **)malloc(sizeof(Node *) * MAX_NEAR);
            lastNode->nextLevel = newNode;
        }
        if(i == 0){
            newNode->nextLevel = NULL;
        }
        // 每一层要连接m-邻近节点，调用ConnectNode函数
        ConnectNode(G, newNode, i);
        // 注意第0层连接NULL
        lastNode = newNode;
    }
    return newNode;
}

// 连接新节点与第level层的m-邻近节点
void ConnectNode(HNSW_Graph *G, Node *newNode, int level)
{
    //critical and hard!
}

// 搜索m-邻近节点
Node *Search(HNSW_Graph *G)
{
    //critical and hard!
}

// 随机插入层数
int RandomLevel()
{
    double u = (double)rand() / RAND_MAX;
    int level = (int)(-log(u) / log(2.0) * RANDOMWeight);
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
