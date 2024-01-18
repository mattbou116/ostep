#include <stdio.h>
#include <stdlib.h>

// would probably be quicker to read all the decodings into a buffer, and then
// write them all together.

void print_decoding(int count, char term) {
    for (int i = 0; i < count; i++) {
        putc(term, stdout);
    }
}

void decode_file(FILE* fp) {
    int count = 0;
    char term = '\0';

    while (!feof(fp)) {
        // read the count
        fread(&count, (sizeof count), 1, fp);
        // attempt to read the term
        if (!feof(fp)) {
            fread(&term, (sizeof term), 1, fp);
        } else {
            // read a line feed character, can just ignore.
            return;
        }
        print_decoding(count, term);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("wunzip: file1 [file2 ...]\n");
        exit(EXIT_FAILURE);
    }

    FILE* fp;

    // loop through the files
    for (int i=1; i < argc; i++) {
        fp = fopen(argv[i], "rb");
        if (fp == NULL) {
            perror("Error");
            printf("wunzip: cannot open file <%s>", argv[i]);
            continue;
        }

        decode_file(fp);
        fclose(fp);
    }

    return EXIT_SUCCESS;
}
