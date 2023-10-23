#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "optimize.h"
#include "translate.h"
#include "errors.h"

int main() {
    // open brainfuck code
    char *code = read_file("test.bf");
    if(code == NULL) {
        printf("Error reading input file\n");
        return 1;
    }
    // minify
    char *minified = minify(code);
    free(code);
    // compress
    char *compressed = compress(minified);
    free(minified);

    // translate
    int err = translate(compressed, "test.asm");
    free(compressed);

    if(err != SUCCESS) {
        printf("Encountered an error during translation:\n -> ");
        switch(err) {
            case ERROR_FILE_INACCESSIBLE:
                printf("Failed to open output file '%s'\n", "test.asm");
                break;
            case ERROR_STACK_OVERFLOW:
                printf("Exceeded maximum loop depth\n");
                break;
            case ERROR_UNCLOSED_LOOP:
                printf("Encountered an unclosed loop\n");
                break;
            case ERROR_UNMATCHED_LOOP:
                printf("Encountered an unmatched loop closure\n");
                break;
        }
        return 1;
    }

    // assemble
    // link

    return 0;
}