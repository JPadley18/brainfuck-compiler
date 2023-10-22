#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "optimize.h"
#include "translate.h"

int main() {
    // open brainfuck code
    char *code = read_file("test.bf");
    // minify
    char *minified = minify(code);
    free(code);
    // compress
    char *compressed = compress(minified);
    free(minified);

    // translate
    int err = translate(compressed, "test.asm");
    free(compressed);

    printf("Finished with code %d\n", err);
    // assemble
    // link
}