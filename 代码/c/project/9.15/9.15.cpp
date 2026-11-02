#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

void pm(int (*row)[4],int rows)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%d", row[i][j]);
        }
        printf("\n");
    }
}
int main()
{
    int matrix[3][4] = {
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    };

    pm(matrix, 2);
    return 0;
}