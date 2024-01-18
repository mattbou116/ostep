#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void recursive_rev(FILE* in, FILE* out) {
    char* line = NULL;
    size_t len = 0;

    if (getline(&line, &len, in) == -1) {
        free(line);
        return;
    }

    recursive_rev(in, out);
    fprintf(out, "%s", line);
    free(line);
}

int main(int argc, char **argv) {
    FILE* in = stdin;
    FILE* out = stdout;

    // check if input comes from tty
    if (argc == 1) {
        struct stat stdin_stat;
        fstat(fileno(stdin), &stdin_stat);
        if (S_ISFIFO(stdin_stat.st_mode)) {
            fprintf(stderr, "usage: reverse <input> <output>\n");
            fprintf(stderr, "error: invalid arguments.\n");
            exit(1);
        }

    } 


    // check argument count
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        fprintf(stderr, "error: invalid arguments.\n");
        exit(1);
    }

    // ensure input file exists
    if (argc > 1) {
        in = fopen(argv[1], "r");
        if (in == NULL) {
            fprintf(stderr, "error: cannot open file '%s'", argv[1]);
            fprintf(stderr, "error: input file not found.\n");
            exit(1);
        }
    }

    if (argc == 3) {
        // ensure we can write to output file
        out = fopen(argv[2], "w");
        if (out == NULL) {
            fprintf(stderr, "error: cannot open file '%s'", argv[2]);
            exit(1);
        }

        // ensure input and output files are different
        struct stat in_stat;
        struct stat out_stat;
        stat(argv[1], &in_stat);
        stat(argv[2], &out_stat);
        if (in_stat.st_dev == out_stat.st_dev && in_stat.st_ino == out_stat.st_ino) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            fprintf(stderr, "error: invalid arguments.\n");
            exit(1);
        }
    }

    recursive_rev(in, out);
    fclose(in);
    fclose(out);
    return 0; 
}

/* PROBLEM W THIS CODE: checking if input comes from terminal not if it's 
   redirected.

    // attempt 1
    if (argc == 1 && !isatty(fileno(stdin))) {
        printf("HERE1\n");
        fprintf(stderr, "usage: reverse <input> <output>\n");
        fprintf(stderr, "error: invalid arguments.\n");
        exit(1);
    } 

    // attempt 2 -- ?
    if (argc == 1) {
        printf("HERE2\n");
        int seek_errc = fseek(stdin, 0, SEEK_END);
        int tell_errc = ftell(stdin);
        if (seek_errc != 0 || tell_errc != 0) { 
            fprintf(stderr, "usage: reverse <input> <output>\n");
            fprintf(stderr, "error: invalid arguments.\n");
            exit(1);
        } else rewind(stdin);
    }

*/
