/*
   - Write a program that calls fork and then execs /bin/ls
   - Try to do with some other exec variants
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void ls_execl() {
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "error: fork failed\n");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child
        int rc_exec = execl("/bin/ls", "ls", "-l", "-a", (char* )NULL);
        if (rc_exec < 0) {
            perror("error");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("parent\n");
    }
}

void ls_execlp() {
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "error: fork failed\n");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child
        int rc_exec = execlp("ls", "ls", "-lh", "-a", (char* )NULL);
        if (rc_exec < 0) {
            perror("error");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("parent\n");
    }
}

void ls_execle() {{
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "error: fork failed\n");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child
        char* env[2] = {"BACKGROUNDS=/home/mattbou/Pictures/bgs", NULL};
        int rc_exec = execle("bs.sh", "bs.sh", "-la", (char* )NULL, env);
        if (rc_exec < 0) {
            perror("error");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("parent\n");
    }
}

}

int main(int argc, char **argv) {
    // ls_execl();
    // ls_execlp();
    ls_execle();
}
