#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

void printd(int output_fd, int n)
{
    if (n < 0)
    {
        write(output_fd, "-", 1);
        printd(output_fd, -n);
        return;
    }
    else if (n == 0)
    {
        return;
    }
    else
    {
        printd(output_fd, n / 10);
    }

    char lastDigit = n % 10 + '0';
    write(output_fd, &lastDigit, 1);
}

int main(int argc, char *argv[])
{

    int num_processes = argc - 1;

    //create pipes for all children
    int pipes[num_processes][2];

    if(pipe(pipes[num_processes]) == -1){
        perror("Error");
        return EXIT_FAILURE;
    } 

    for (int i = 0; i < num_processes; i++)
    {
        pipe(pipes[i]);
    }

    for (int i = 0; i < num_processes; i++)
    {
        int pid = fork();
        if (pid == -1)
        {
            printf("Error with creating process\n");
            return 1;
        }
        if (pid == 0)
        {
            // Child process

            for (int i = 0; i < num_processes; i++)
            {
                close(pipes[i][0]);
            }

            FILE *fp = fopen(argv[i + 1], "rb");
            if (!fp)
            {
                printf("child %d with file %s", i, argv[i + 1]);
                fflush(stdout);
                perror(" - ");
                write(pipes[i][1], "0\n",2);
            }
            else
            {
                //sum integers from file
                int arr[100];
                int n = fread(&arr, sizeof(int), 100, fp);

                int sum_child = 0;
                for (int i = 0; i < n; i++)
                {
                    sum_child += arr[i];
                }

                printd(pipes[i][1], sum_child);
                write(pipes[i][1], "\0", 1);
                fclose(fp);
            }
            for (int i = 0; i < num_processes; i++)
            {
                close(pipes[i][1]);
            }

            exit(0);
        }
    }

    for (int i = 0; i < num_processes; i++)
    {
        close(pipes[i][1]);
    }

    for (int i = 0; i < num_processes; i++)
    {
        wait(NULL);
    }

    //sum from all children
    int sum = 0;
    for (int i = 0; i < num_processes; i++)
    {
        char buffer[50];
        read(pipes[i][0], buffer, 50);
        
        printf("child %d - %d\n", i, atoi(buffer));

        sum += atoi(buffer);
    }

    printf("%d\n", sum);

    for (int i = 0; i < num_processes; i++)
    {
        close(pipes[i][0]);
    }

    return 0;
}