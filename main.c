#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "optimize.h"

// Number of byte cells to give Brainfuck executable
#define NUM_CELLS 30000
// Maximum number of nested loops allowed
#define MAX_LOOP_DEPTH 1024

int main() {
    // open brainfuck code
    char *code = read_file("test.bf");
    // minify
    char *minified = minify(code);
    free(code);
    // compress
    char *compressed = compress(minified);
    free(minified);

    printf("%s\n", compressed);
    free(compressed);
    // translate
    // assemble
    // link
}