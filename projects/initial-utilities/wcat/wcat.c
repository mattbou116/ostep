#include <stdio.h>
#include <stdlib.h>

// "./wcat" counts as an argument
// "$ ./wcat" -> argc = 1
// "$ ./wcat myfile.txt" -> argc = 2

void print_file(FILE* fp) {
    char c;
    c = fgetc(fp);
    while (c != EOF) {
        putchar(c);
        c = fgetc(fp);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        exit(0);
    } 

    FILE *fp;
    for (int i = 1; i < argc; i++) {
        fp = fopen(argv[i], "r");

        if (fp == NULL) {
            perror("Error");
            printf("wcat: cannot open file\n");
            exit(1);
        }
        print_file(fp);
        fclose(fp);
    }

    return 0;
}
