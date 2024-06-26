#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "../inc/definition.h"
#include "../inc/structures.h"

char dataset_filepath[FILEPATHLEN];
char search_filepath[FILEPATHLEN];
Node *nodeList[NODESUM];
clock_t start_build, end_build, start_HNSW_search, end_HNSW_search, start_BruteForce_search, end_BruteForce_;
int main()
{
    HNSW_Graph *HNSW_Graph_Instance;
    InitalGraph(&HNSW_Graph_Instance);
    char *datasets[7] = {"all", "aircraft", "clothing", "flower", "food", "logo", "custom"};
    int main_choice = 1;
    while ((main_choice != 0))
    {
        // ShowMenu(0, NULL);
        scanf("%d", &main_choice);
        switch (main_choice)
        {
        case 0: //
            break;
        case 1:
        {
            // ShowMenu(1,NULL);
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
                    // HNSW_Graph_Instance->nodeCount = 1;
                    //  建图，dataset_filepath为文件路径，HNSW_Graph_Instance为图
                    float progress = 0.0;
                    int nodeSum = (dataset_choice == 1) ? 5 * NODESUM : NODESUM;
                    start_build = clock();
                    for (int i = 0; i < nodeSum; i++)
                    {
                        InsertNode(HNSW_Graph_Instance, i, dataset_filepath, dataset_filepath, nodeList);
                        // progress = (float)i / nodeSum * 1000;
                        // printf("\rBuilding Graph: %.2f%%", progress);
                        // fflush(stdout);
                        // printf("\033[K");
                    }
                    end_build = clock();
                    double build_time_spent = (double)(end_build - start_build) / CLOCKS_PER_SEC;
                    printf("%10f\n", build_time_spent);
                    printf("\n");
                    // printf("\nGraph Build Finish!\n");
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
            // ShowMenu(2,NULL);
            int search_choice;
            scanf("%d", &search_choice);
            if (search_choice == 0)
            {
                break;
            }
            else if (search_choice > 7 || search_choice < 0)
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
                    snprintf(search_filepath, FILEPATHLEN, "%s%s%s", "../data/preprocess/search/", datasets[search_choice - 1], "/%d.txt");
                    char ls_code[100];
                    snprintf(ls_code, 100, "%s%s", "ls ../data/preprocess/search/", datasets[search_choice - 1]);
                    // ShowMenu(3, ls_code);
                    int index;
                    scanf("%d", &index);
                    Node **ReturnList = (Node **)malloc(sizeof(Node *) * SEARCH_NUM);

                    start_HNSW_search = clock();
                    ReturnList = Search(HNSW_Graph_Instance, index, search_filepath, dataset_filepath);
                    end_HNSW_search = clock();
                    double HNSW_search_time_spent = (double)(end_HNSW_search - start_HNSW_search) / CLOCKS_PER_SEC;
                    printf("%10f\n", HNSW_search_time_spent);
                    /*
                    for (int i = 0; i < SEARCH_NUM; i++)
                    {
                        printf("ReturnList[%d]=%d Distance = %f\n", i, ReturnList[i]->data, ReturnList[i]->distance);
                    }
                    */
                    Node *tempNode = (Node *)malloc(sizeof(Node));

                    tempNode->data = index;
                    float result[SEARCH_NUM];
                    int resultIndex[SEARCH_NUM];
                    for (int i = 0; i < SEARCH_NUM; i++)
                    {
                        result[i] = 0;
                        resultIndex[i] = 0;
                    }
                    start_BruteForce_search = clock();
                    BruteForceSearch(tempNode, result, resultIndex, search_filepath, dataset_filepath);
                    end_BruteForce_ = clock();
                    double BruteForce_search_time_spent = (double)(end_BruteForce_ - start_BruteForce_search) / CLOCKS_PER_SEC;
                    printf("%10f\n", BruteForce_search_time_spent);
                    int equal_count = 0;
                    for (int i = 0; i < SEARCH_NUM; i++)
                    {
                        for (int j = 0; j < SEARCH_NUM; j++)
                        {
                            if (ReturnList[i]->data == resultIndex[j])
                            {
                                equal_count++;
                            }
                        }
                    }
                    double recall = (double)equal_count / SEARCH_NUM;
                    printf("%10f\n", recall);

                    /*for (int i = 0; i < SEARCH_NUM; i++)
                    {
                        printf("BruteForceSearch[%d]=%d %f\n", i, resultIndex[i], result[i]);
                    }
                    */
                    // 搜索返回结果，然后处理结果（返回搜索图与m临近图到output文件夹）
                }
            }
            break;
        }
        }
    }
    DeleteGraph(&HNSW_Graph_Instance, nodeList);
}
