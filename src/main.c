#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "definition.h"
#include "structures.h"

char dataset_filepath[FILEPATHLEN];
char search_filepath[FILEPATHLEN];
int main()
{
    HNSW_Graph *HNSW_Graph_Instance;
    InitalGraph(&HNSW_Graph_Instance);
    char *datasets[7] = {"all", "aircraft", "clothing", "flower", "food", "logo", "custom"};
    int main_choice = 1;
    while ((main_choice != 0))
    {
        ShowMenu(0);
        scanf("%d", &main_choice);
        switch (main_choice)
        {
        case 0: //
            break;
        case 1:
        {
            ShowMenu(1);
            int dataset_choice;
            scanf("%d", &dataset_choice);
            if (dataset_choice == 0)
            {
                break;
            }
            else if (dataset_choice > 7 || dataset_choice < 0)
            {
                printf("Invalid choice\n");
                break;
            }
            else
            {
                if (HNSW_Graph_Instance->nodeCount == 0)
                {
                    snprintf(dataset_filepath, FILEPATHLEN, "%s%s%s", "../data/preprocess/", datasets[dataset_choice - 1], "/%d.txt");
                    HNSW_Graph_Instance->nodeCount = 1;
                    // 建图，dataset_filepath为文件路径，HNSW_Graph_Instance为图
                    /*
                    for (int i = 0; i < 1000; i++)
                    {
                        InsertNode(HNSW_Graph_Instance, i, dataset_filepath, dataset_filepath);
                    }
                    printf("Done!");
                    */
                    // BUG:layer=0 next?
                }
                else
                {
                    printf("The graph is already built, if you want to rebuild the graph, please restart the program\n");
                    break;
                }
            }
        }
        break;
        case 2:
        {
            ShowMenu(2);
            int search_choice;
            scanf("%d", &search_choice);
            if (search_choice == 0)
            {
                break;
            }
            else if (search_choice > SEARCH_OBJECT_NUM + 2 || search_choice < 0)
            {
                printf("Invalid choice\n");
                break;
            }
            else
            {
                if (HNSW_Graph_Instance->nodeCount == 0)
                {
                    printf("Please build the graph first\n");
                    break;
                }
                else
                {
                    if (search_choice == 1)
                    {
                        snprintf(search_filepath, FILEPATHLEN, "%s", "../data/preprocess/search");
                        printf("\nFiles:\n");
                        system("ls ../data/preprocess/search");
                        printf("Please input the index:");
                        int index;
                        scanf("%d", &index);

                        Node **ReturnList = (Node **)malloc(sizeof(Node *) * SEARCH_NUM);
                        ReturnList = Search(HNSW_Graph_Instance, index, search_filepath, dataset_filepath);
                        for (int i = 0; i < SEARCH_NUM; i++)
                        {
                            printf("ReturnList[%d]=%d\n", i, ReturnList[i]->data);
                        }
                        Node *tempNode = (Node *)malloc(sizeof(Node));
                        tempNode->data = index;
                        float result[SEARCH_NUM];
                        int resultIndex[SEARCH_NUM];
                        BruteForceSearch(tempNode, result, resultIndex, search_filepath, dataset_filepath);
                        for (int i = 0; i < SEARCH_NUM; i++)
                        {
                            printf("BruteForceSearch[%d]=%d\n", i, resultIndex[i]);
                        }
                        // 查找，index为文件索引，search_filepath为文件路径
                    }
                    else
                    {
                        snprintf(search_filepath, FILEPATHLEN, "%s", "../data/preprocess/search/%d.txt");
                        int index = search_choice;

                        Node **ReturnList = (Node **)malloc(sizeof(Node *) * SEARCH_NUM);
                        ReturnList = Search(HNSW_Graph_Instance, index, search_filepath, dataset_filepath);
                        for (int i = 0; i < SEARCH_NUM; i++)
                        {
                            printf("ReturnList[%d]=%d\n", i, ReturnList[i]->data);
                        }
                        Node *tempNode = (Node *)malloc(sizeof(Node));

                        tempNode->data = index;
                        float result[SEARCH_NUM];
                        int resultIndex[SEARCH_NUM];
                        for (int i = 0; i < SEARCH_NUM; i++)
                        {
                            result[i] = 0;
                            resultIndex[i] = 0;
                        }
                        BruteForceSearch(tempNode, result, resultIndex, search_filepath, dataset_filepath);
                        for (int i = 0; i < SEARCH_NUM; i++)
                        {
                            printf("BruteForceSearch[%d]=%d %f\n", i, resultIndex[i], result[i]);
                        }
                        // 查找，index为文件索引，search_filepath为文件路径

                        // 搜索返回结果，然后处理结果（返回搜索图与m临近图到output文件夹）
                    }
                }
            }
        }
        break;
        }
    }
}