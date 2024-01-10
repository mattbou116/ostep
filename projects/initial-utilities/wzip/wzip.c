#include <stdio.h>
#include <stdlib.h>

// don't care for fwrite err code atm.
void print_encoding(int* count, char* term) {
    // printf("encoding: [count]=%d ; [char]=%c\n", *count, *term);
    fwrite(count, 4, 1, stdout);
    fwrite(term, 1, 1, stdout);
}

void encode_file(FILE* fp, char* cur, char* prev, int* count) {
    *cur = fgetc(fp);

    // check if this is the first file we are encoding.
    if (*prev == '\0')
        *prev = *cur;

    // if we reached the end of the file, wait to see if next file has the same
    //      char as the last one in this file.
    while (*cur != EOF) {
        // compress contiguous repeats.
        if (*cur == *prev) *count = *count + 1;
        else {
            // encode if we've reached a different char.
            print_encoding(count, prev);
            *count = 1;
        }
        *prev = *cur;
        *cur = fgetc(fp);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        exit(EXIT_FAILURE);
    }

    FILE* fp;
    char cur, prev;
    prev = '\0';
    int count = 0;

    // loop through the files
    for (int i=1; i < argc; i++) {
        fp = fopen(argv[i], "r");
        if (fp == NULL) {
            perror("Error");
            printf("wzip: cannot open file <%s>", argv[i]);
            continue;
        }

        // reset cur so it's not EOF.
        cur = '\0';
        encode_file(fp, &cur, &prev, &count);
        // if we've reached the last file, then just encode what we have.
        if (i == argc-1) {
            print_encoding(&count, &prev);
        }
        fclose(fp);
    }

    return EXIT_SUCCESS;
}
