#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <pthread.h>

int sumOfNums(int num);
void* SumOfNumsInFile(void *arg);

int main(int argc,char** argv)
{
    pthread_t *th = malloc(sizeof(pthread_t) * (argc - 1));
    for(int i=0;i<argc-1;i++)
    {
        pthread_create(&th[i],NULL,SumOfNumsInFile,argv[i+1]);
    }
    int sum = 0;
    int *num;
    
    for(int i=0;i<argc - 1;i++)
    {
        pthread_join(th[i],(void*)num);
        if(*num != -1)
        {
            sum += *num;
        }
    }
    
    fprintf(stdout,"From all:%d\n",sum);
    free(th);
    return 0;
}

void* SumOfNumsInFile(void *arg)
{
    int i = 0;
    int num;
    int *sum = malloc(sizeof(int));
    *sum = 0;
    FILE *f;
    f = fopen((char*)arg,"r");
    if(f == NULL)
    {
        perror((char*)arg);
        *sum = -1;
        return (void*)sum;
    }
    while(fscanf(f,"%d",&num) != EOF)
    {
        *sum += sumOfNums(num);
    }
    fclose(f);
    fprintf(stdout,"%s - %d\n",(char*)arg,*sum);
    return (void*)sum;
}
int sumOfNums(int num)
{
    int sum = 0;
    while(num != 0)
    {
        sum += num%10;
        num /= 10;
    }
    return sum;
}