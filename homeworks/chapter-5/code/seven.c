/*
   - write a program that creates a child process
   - close stdout in the child process
   - what happens if child calls printf() after closing the stdout descriptor?
        - prediction: printf won't work in child?
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv){

    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "error: fork failed\n");
        perror("error");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child
        int rc_close = close(fileno(stdout));
        if (rc_close != 0) {
            fprintf(stderr, "error: close failed\n");
            perror("error");
            exit(EXIT_FAILURE);
        }
        printf("can you hear me? -child\n");
    }  else {
        // parent
        printf("hello from parent\n");
    }

    return 0;
}
