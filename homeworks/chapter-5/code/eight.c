/*
    - write a program that creates two child
    - connect the stdout of one child to the stdin of the other using pipe() 
        syscall
    - man pipe: "pipe() creates a unidirectional data channel that can be
        used for interprocess communication"
        - "ls | wc -w"
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe error");
        exit(1);
    }


    // create first child
    pid_t cpid1 = fork();

    if (cpid1 < 0) {
        perror("fork 1 error");
        exit(1);
    } else if (cpid1 == 0) {
        // first child writes to pipe
        printf("<pid=%d> first child -- execute ls\n", getpid());

        // close the read channel
        close(pipefd[0]);
        dup2(pipefd[1], fileno(stdout));

        // write to pipe
        execlp("ls", "ls", (char* )NULL);

        close(pipefd[1]);

        // exit first child
        exit(0);
    } else {
        // parent
        printf("<pid=%d> parent\n", getpid());

        // wait for first child to write to pipe
        waitpid(cpid1, NULL, 0);

        int rc_two = fork();
        if (rc_two < 0) {
            perror("fork 2 error");
            exit(1);
        } else if (rc_two == 0) {
            // second child reads from pipe
            printf("<pid=%d> second child -- execute wc -w input from ls\n", getpid());

            // close write channel
            close(pipefd[1]);
            dup2(pipefd[0], fileno(stdin));

            execlp("wc", "wc", "-w", (char* )NULL);
            close(pipefd[0]);

            // exit second child
            exit(0);
        } else {
            // parent
            printf("<pid=%d> parent\n", getpid());
        }
    }

    return 0;
}
