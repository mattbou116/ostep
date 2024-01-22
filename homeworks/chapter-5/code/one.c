/*
    - Write a program that calls fork()
    - Before calling fork(), set a var.
    - What value is var in child proecess?
    - What happens to var when both child and parent change var?
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void child_access(int* x) {
    printf("FORKING!\n");
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "error: fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child
        printf("child <pid=%d> -- x=%d\n", getpid(), *x);
        exit(0);
    } else {
        // parent
        printf("parent <pid=%d> -- x=%d\n", getpid(), *x);
    }
}

void both_set(int* x) {
    printf("FORKING!\n");
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "error: fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child
        *x = 0;
        printf("child <pid=%d> -- x=%d\n", getpid(), *x);
        exit(0);
    } else {
        // parent
        *x = 200;
        printf("parent <pid=%d> -- x=%d\n", getpid(), *x);
    }
}

int main(int argc, char** argv) {
    int x = 100;
    printf("parent <pid=%d> -- x=%d\n", getpid(), x);
    // child_access(&x);
    both_set(&x);
}
