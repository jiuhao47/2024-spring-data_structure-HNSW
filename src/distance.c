#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../inc/definition.h"
#include "../inc/structures.h"

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
float Distance(int a, int b)
{
    float distance;
    char filepath1[FILEPATHLEN];
    char filepath2[FILEPATHLEN];
    sprintf(filepath1, FILEPATH, a);
    sprintf(filepath2, FILEPATH, b);
    float vector1[VECTORSIZE];
    float vector2[VECTORSIZE];
    ReadVectorFromFile(filepath1, vector1, VECTORSIZE);
    ReadVectorFromFile(filepath2, vector2, VECTORSIZE);
    distance = CosineDistance(vector1, vector2, VECTORSIZE);
    return distance;
}

int main()
{
    float distance = 0.0;
    distance = Distance(1, 2);
    printf("Distance: %f\n", distance);
    return 0;
}