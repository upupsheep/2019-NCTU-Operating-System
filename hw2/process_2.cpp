#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
using namespace std;
pid_t wpid;
int status;
int main(int argc, char* argv[]) {
    int pidx = 0, pidy = 0, pidz = 0, pCount = 8;
    printf("create main process %d\n", getpid());
    printf("process %d create process %d\n", getpid(), pidx = fork());
    printf("process %d create process %d\n", getpid(), pidy = fork());
    printf("process %d create process %d\n", getpid(), pidz = fork());

    waitpid(pidx, &status, 0);
    waitpid(pidy, &status, 0);
    waitpid(pidz, &status, 0);
    printf("process %d deaded its child process %d %d %d\n", getpid(), pidx,
           pidy, pidz);

    return 0;
}