/*
    Benchmark tool to measure the performance of a context switch.
    - gettimeofday() -- time in microsecnds since 1970.
        - NOTE: having some bugs with gettimeofday or my code is not working
            as expected! yay!
    - sched_setaffinity() -- useful in binding process to a specific processor.

    Attempt to measure the performance of two processes communicating through
    a UNIX pipe. (* pipefd[0] : read end; pipefd[1] : write end *)
*/
#define _GNU_SOURCE
#define DURATION 100000

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    int p1[2];
    int p2[2];
    char wbuf1 = '1';
    char wbuf2 = '2';
    char rbuf1;
    char rbuf2;
    struct timeval tv1;
    struct timeval tv2;
    struct timeval tv3;
    struct timeval tv4;
    cpu_set_t cps;
    CPU_ZERO(&cps);
    CPU_SET(0, &cps);

    if (pipe2(p1, O_NONBLOCK) == -1) {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }

    if (pipe2(p2, O_NONBLOCK) == -1) {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }

    pid_t ppid = getpid();
    // set parent to only operate on 0th cpu
    sched_setaffinity(ppid, sizeof(cps), &cps);
    pid_t cpid = fork();
    if (cpid < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    } else if (cpid > 0) {
        // parent
        // set child to only operate on 0th cpu
        sched_setaffinity(cpid, sizeof(cps), &cps);
        gettimeofday(&tv1, NULL);
        for (int i=0; i<DURATION; i++) {
            // write of < 512 byte is atomic
            write(p1[1], &wbuf1, 1);
            // OS blocks until something is written to the pipe
            read(p2[0], &rbuf2, 1);
        }
        gettimeofday(&tv2, NULL);
        long long diff = (long long) (tv2.tv_usec - tv1.tv_usec);
        double avg = (double) diff / DURATION;
        // printf("<pid=%d> parent time 1: %lld (ms)\n", ppid, (long long) tv1.tv_usec);
        // printf("<pid=%d> parent time 2: %lld (ms)\n", ppid, (long long) tv2.tv_usec);
        printf("<pid=%d> parent time difference: %lld (microseconds)\n", ppid, diff);
        printf("<pid=%d> parent avg time: %f (microseconds\n", ppid, avg);
    } else {
        // child
        gettimeofday(&tv3, NULL);
        for (int i=0; i<DURATION; i++) {
            // OS blocks until something is written to the pipe
            read(p1[0], &rbuf1, 1);
            // write of < 512 byte is atomic
            write(p2[1], &wbuf2, 1);
        }
        gettimeofday(&tv4, NULL);
        long long diff = (long long) (tv4.tv_usec - tv3.tv_usec);
        double avg = (double) diff / DURATION;
        cpid = getpid();
        // printf("<pid=%d> child time 1: %lld (ms)\n", cpid, (long long) tv1.tv_usec);
        // printf("<pid=%d> child time 2: %lld (ms)\n", cpid, (long long) tv2.tv_usec);
        printf("<pid=%d> child time difference: %lld (microseconds)\n", cpid, diff);
        printf("<pid=%d> child avg time: %f (microseconds)\n", cpid, avg);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
