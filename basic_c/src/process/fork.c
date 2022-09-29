#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{

    printf("[%d]:Begin\n", getpid());
    fflush(NULL);
    __pid_t cur_pid = fork();//just copy current progress of parent process
                            //buffer will also be copied(all resources belong to parent process will be copied)
    if (cur_pid < 0)
    {
        printf("error!\n");
        return -1;
    }
    else if (cur_pid == 0)
        printf("[%d]:Child\n", getpid());
    else
        printf("[%d]:Parent\n", getpid());

    printf("[%d]:End\n", getpid());

    return 0;
}