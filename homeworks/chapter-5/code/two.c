/*
    - Write a program that opens a file using open() syscall
    - Then call fork()
    - Can both child and parent access fd returned by open()?
        - note: file descriptors are just index that refers to a table
            of open files particular to a process (open -- linux manual).
        - note: open return fs (lowest possible fd)
        - prediction: yes (fork copies process data?)
        - answer: yes
    - What happens to when both processes concurrently write to the same file?
        - O_ASYNC flag?
        - O_CLOEXEC flag?
        - prediction: bad things (without mutexes)
        - answer: arbitrary order of write operations.
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
   int fd = open("./bs.txt", O_WRONLY | O_CREAT | O_APPEND);
   printf("parent (before fork) <pid=%d>\n", getpid());
   int rc = fork();
   if (rc < 0) {
        fprintf(stderr, "error: fork failed\n");
        exit(1);
   } else if (rc == 0) {
        // child
        char* msg = "hello from child!\n";
        for (int i = 0; i < 1000; i++) {
           write(fd, msg, strlen(msg));
        }
   } else {
        // parent
        char* msg = "hello from parent!\n";
        for (int i = 0; i < 1000; i++) {
           write(fd, msg, strlen(msg));
        }
   }
   close(fd);
}
