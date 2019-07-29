#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
    pid_t childpid;
    int fd[2];
    char buffer[32];
    int status;

    pipe(fd);
    childpid = fork();

    if (childpid < 0) {
        printf("fork failed\n");
    } else if (childpid == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        printf("NCTU");
        sprintf(buffer, "NTU");
    } else {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        scanf("%s", buffer);
        printf("%s NTHU\n", buffer);
        waitpid(childpid, &status, 0);
    }

    return 0;
}