#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool search_line(char* line, char* term, ssize_t line_len) {
    int line_idx = 0;
    int term_idx = 0;
    int term_len = strlen(term);
    for (; line_idx<line_len; line_idx++) {
        if (term_idx == term_len) {
            return true;
        } else if (term_idx < term_len && line[line_idx] == term[term_idx]) {
            term_idx++;
        } else {
            term_idx = 0;
        }
    }
    return false;
}

void search_file(FILE* fp, char* term) {
    size_t n = 0;
    char* line = NULL;
    ssize_t len = getline(&line, &n, fp);
    while (len != -1) {
        if(search_line(line, term, len))
            printf("%s", line);
        len = getline(&line, &n, fp);
    }
    free(line);
}

void invalid_args() {
    printf("wgrep: searchterm [file ...]\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    FILE* fp;

    if (argc < 2) {
        invalid_args();
    } else if (argc == 2) {
        if (feof(stdin) == 0) {
            fp = stdin;
        } else invalid_args();
    } else if (argc == 3) {
        fp = fopen(argv[2], "r");
        if (fp == NULL) {
            perror("Error");
            printf("wgrep: cannot open file\n");
            exit(EXIT_FAILURE);
        }
    } else {
        invalid_args();
    }

    char* term = argv[1];
    search_file(fp, term);

    return EXIT_SUCCESS;
}
