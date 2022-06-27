#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


void *routine(void *arg)
{
    char *filename = (char *)arg;
    int *result = malloc(sizeof(int));
    *result = 0;

    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("%s", filename);
        fflush(stdout);
        perror(" - ");
    }
    else
    {
        int arr[100];
        int n = fread(&arr, sizeof(int), 100, fp);

        for (int i = 0; i < n; i++)
        {
            *result += arr[i];
        }

        fclose(fp);
    }
    return (void *)result;
}

int main(int argc, char *argv[])
{
    int N = argc - 1;

    pthread_t threads[N];

    for (int i = 0; i < N; i++)
    {

        if (pthread_create(threads + i, NULL, routine, argv[i + 1]) != 0)
        {
            perror("Error creating thread");
            return 1;
        }
    }

    int totalSum = 0;
    for (int i = 0; i < N; i++)
    {
        int *result;
        if (pthread_join(threads[i], (void*)&result) != 0)
        {
            perror("Error joining thread");
            return 1;
        }
        totalSum += *result;
        free(result);
    }

    printf("%d\n", totalSum);

    return 0;
}