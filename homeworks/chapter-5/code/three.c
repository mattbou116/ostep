/* 
   - Write another program using fork()
   - Child process should print "hello"
   - Parent process should print "goodbye"
   - Make it so child prints first without using wait()
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

void with_wait() {
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "error: fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child
        printf("hello ");
        exit(0);
    } else {
        // parent
        int rc_wait = wait(NULL);
        if (rc_wait < 0) {
            fprintf(stderr, "error: wait failed\n");
            exit(1);
        }
        printf("goodbye\n");
    }
}

void without_wait() {
    // should attempt to do without wait again
    // file as shared memory? use some kind of process lock?
}


int main(int argc, char** argv) {
    printf("<pid=%d> parent (before call)\n", getpid());
    with_wait();
    printf("<pid=%d> parent (after call)\n", getpid());
    // without_wait();
    return 0;
}
