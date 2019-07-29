#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
using namespace std;
int status = 0;

int main() {
    int n = 10;
    pid_t pid = fork();
    if (pid == 0) printf("process pid %d create\n", getpid());
    for (int i = 0; i < n; i++) {
        if (pid == 0) {
            pid_t p_pid = getpid();
            // printf("[son] pid %d from [parent] pid %d\n", getpid(),
            // getppid());
            // printf("process pid %d create\n", getpid());
            pid = fork();

            if (getppid() == p_pid)
                // printf("[son] pid %d from [parent] pid %d\n", getpid(),
                //        getppid());
                printf("process pid %d create\n", getpid());
            p_pid = getpid();
            // exit(0);
        }
    }
    // printf("[son] pid %d from [parent] pid %d\n", getpid(), getppid());
    pid_t end_pid = wait(NULL);
    if (end_pid > 0) printf("process pid %d dead\n", end_pid);
    return 0;
}