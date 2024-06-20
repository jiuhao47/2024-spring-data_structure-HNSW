typedef int NodeDataType;
typedef struct Node
{
    NodeDataType data;      // 数据域
    struct Node **pList;    // 同层m临近数组
    struct Node *nextLevel; // 下一层
} Node;

typedef struct HNSW_Graph
{
    Node **pEntryPointList; // 入口点数组
    int highestLevel;       // 当前最高层数
    int nodeCount;          // 当前节点数
    // maxLevel defined
} HNSW_Graph;