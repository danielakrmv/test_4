#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int N, M;
int **mat;

int intcmp(const void* a, const void*b)
{
    int ia = *(int*)a;
    int ib = *(int*)b;

    return ia - ib;
}

void *routine(void *arg)
{
    int *arr = (int *)arg;

    qsort(arr, M, sizeof(int), intcmp);
}


int main()
{
    scanf("%d %d", &N, &M);

    mat = malloc(sizeof(int *) * N);
    for (int i = 0; i < N; i++)
    {
        mat[i] = malloc(sizeof(int) * M);
    }

    int *rowNums = malloc(sizeof(int)*N);
    for (int i = 0; i < N; i++)
    {
        char bufferstring[10];
        scanf("%s", bufferstring);
        scanf("%d", rowNums + i);

        for (int j = 0; j < M; j++)
        {
            scanf("%d", mat[i] + j);
        }
    }

    pthread_t threads[N];
    int *threadNums[N];

    for (int i = 0; i < N; i++)
    {

        if (pthread_create(threads + i, NULL, routine, mat[i]) != 0)
        {
            perror("Error creating thread");
            return 1;
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Error joining thread");
            return 1;
        }

    }

    printf("Please enter filename:\n");
    char filename[100];
    scanf("%s", filename);

    // FILE *fp = fopen(filename, "w+");
    // if(!fp)
    // {
    //     perror("file");
    //     return 1;
    // }

    for (int i = 0; i < N; i++)
    {

        // fwrite(&buff, sizeof(char), strlen(buff), fp);
        // fputs(buff,fp);
        // fwrite(&rowNums[i], sizeof(int), 1, fp);

        printf("ROW ");
        printf("%d ", rowNums[i]);
        for (int j = 0; j < M; j++)
        {
            printf("%d ", mat[i][j]);
        }
        putchar('\n');
    }

    return 0;
}