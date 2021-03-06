#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

pid_t pid;
int status;

void sigchld_handler(int signo)
{
    pid = wait(&status);
    if (pid != -1)
        printf("PARENT: child with PID=%d terminated               with exit code %d\n", pid, WEXITSTATUS(status));
}

int main(void)
{
    struct sigaction chld;
    chld.sa_handler = sigchld_handler;
    sigemptyset(&chld.sa_mask);
    chld.sa_flags = 0;

    sigaction(SIGCHLD, &chld, NULL);

    int i, n;
    for (i = 1; i <= 10; i++)
    {
        pid = fork();

        if (pid == 0)
        {
            printf("CHILD no. %d (PID=%d) working ... \n", i, getpid());
            sleep(15); // child working ...
            printf("CHILD no. %d (PID=%d) exiting ... \n", i, getpid());
            exit(0);
        }
    }
    for (i = 1; i <= 4; i++)
    {
        printf("PARENT: working hard (task no. %d) ...\n", i);
        n = 20;
        while ((n = sleep(n)) != 0)
            ;
        printf("PARENT: end of task no. %d\n", i);
        printf("PARENT: waiting for child no. %d ...\n", i);
        
    }
    exit(0);
}