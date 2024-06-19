#include <stdio.h>
#include <stdlib.h>
#include "../inc/definition.h"
#include "../inc/structures.h"

// 初始化HNSW
void InitalGraph(HNSW_Graph *G)
{
    G = (HNSW_Graph *)malloc(sizeof(HNSW_Graph));
    G->highestLevel = 0; //事实上此时即使第0层也没有点
    G->nodeCount = 0; //没有点
    G->pEntryPointList = (Node **)malloc(sizeof(Node *) * MAX_LAYER); //入口点数组
}

// 插入新节点
Node *InsertNode(HNSW_Graph *G, NodeDataType data)
{   
    int level = RandomLevel();
    Node *lastNode; //记录上一个节点，用于连接
    Node *newNode = (Node *)malloc(sizeof(Node)); //最顶层节点分配空间
    newNode->pList = (Node **)malloc(sizeof(Node *) * MAX_NEAR); //分配m-邻近节点空间
    if (level > G->highestLevel) //每一层的第一个节点都作为入口点
    {
        G->highestLevel = level;
        G->pEntryPointList[level] = newNode;
    }
    for (int i = level; i >= 0; i--)
    {   
        //给每一层的点分配空间（i不等于level时）
        //连接上一个节点，然后更新lastNode
        //每一层要连接m-邻近节点，调用ConnectNode函数
        //注意第0层连接NULL
    }
    return newNode;
}

// 连接新节点与第level层的m-邻近节点
void ConnectNode(HNSW_Graph *G, Node *newNode, int level)
{
    
}


// 搜索m-邻近节点
Node *Search(HNSW_Graph *G)
{
    
}

// 随机插入层数
int RandomLevel()
{
    int level;

    return level;
}

//计算两个节点之间的距离（当前为空函数）
float Distance(Node *a, Node *b)
{
    float distance;

    return distance;
}