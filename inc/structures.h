typedef int NodeDataType;
typedef struct Node
{
    float distance;         // 距离
    NodeDataType data;      // 数据域
    struct Node **pList;    // 同层m临近数组
    int connectCount;       // 点连接数
    struct Node *nextLevel; // 下一层
} Node;

typedef struct HNSW_Graph
{
    Node **pEntryPointList; // 入口点数组
    int highestLevel;       // 当前最高层数
    int nodeCount;          // 当前节点数
    // maxLevel defined
} HNSW_Graph;
typedef struct SearchList
{
    Node **visitedPointList;    // 访问过节点数组 变长
    int visitedPointCount;      // 访问过节点数
    Node **candidatePointList;  // 候选节点数组 长度为 MAX_NEAR * MAX_NEAR + MAX_NEAR + 1
    Node **candidatePointList2; // 候选节点数组的影子数组
    int candidatePointCount;    // 候选节点数
    int candidatePointCount2;   // 候选节点数
} SearchList;
void InitalGraph(HNSW_Graph **G);
void InsertNode(HNSW_Graph *G, NodeDataType data, char *filepath_a, char *filepath_b, Node *nodeList[]);
void FindNode(HNSW_Graph *G, Node *newNode, int level, SearchList *SL, char *filepath_a, char *filepath_b);
void insertionSort(SearchList *SL, int n);
void InsertVisitedPointList(SearchList *SL, Node *node);
void InsertCandidatePointList(SearchList *SL, Node *node, Node *newNode, char *filepath_a, char *filepath_b);
Node **Search(HNSW_Graph *G, NodeDataType data, char *filepath_a, char *filepath_b);
void DeleteGraph(HNSW_Graph **G, Node *nodeList[]);
int RandomLevel();
float CosineDistance(float *a, float *b, int size);
void ReadVectorFromFile(char *filename, float *vector, int size);
float Distance(Node *a, Node *b, char *filepath_a, char *filepath_b);
int findMinIndex(float *result);
void BruteForceSearch(Node *a, float *result, int *resultIndex, char *filepath_a, char *filepath_b);
void ShowMenu(int choice, char *code);