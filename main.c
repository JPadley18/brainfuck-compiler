#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "util.h"
#include "optimize.h"
#include "translate.h"
#include "errors.h"

#define USAGE_HINT "Usage: bfc [filename] [-o output] [-s]\n"

/**
 * Removes a temporary file
 * 
 * @param file the name of the temporary file to delete
*/
void clean(char *file) {
    if(remove(file)) {
        printf("Failed to remove %s\n", file);
    }
}

/**
 * Runs a system command to generate an object file from the given assembly file
 * 
 * @param asmFile the name of the assembly file to assemble
 * @param outputFile the name of the object file to output to
*/
void assemble(char *asmFile, char *outputFile) {
    char assemble[strlen(outputFile) + strlen(asmFile) + 18];
    sprintf(assemble, "nasm -f elf32 -o %s %s", outputFile, asmFile);
    if(system(assemble)) {
        printf("Assembly failed\n");
        clean(asmFile);
        exit(1);
    }
}

/**
 * Runs a system command to link the generated object file into an executable
 * 
 * @param objInputFile the name of the object file to link
 * @param outputFile the name of the executable file to output
*/
void link(char *objInputFile, char *outputFile) {
    char link[strlen(outputFile) + strlen(objInputFile) + 19];
    sprintf(link, "ld -m elf_i386 -o %s %s", outputFile, objInputFile);
    if(system(link)) {
        printf("Linking failed\n");
        clean(objInputFile);
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    // Parse command line arguments
    char *outputFile = "out";
    // Skip cleanup flag
    int sFlag = 0;

    char c;
    while((c = getopt(argc, argv, "so:")) != -1) {
        switch(c) {
            case 'o':
                outputFile = optarg;
                break;
            case 's':
                sFlag = 1;
                break;
            default:
                return 1;
        }
    }

    char *inputFile = argv[optind];
    if(!inputFile) {
        printf(USAGE_HINT);
        return 1;
    }

    // open brainfuck code
    char *code = read_file(inputFile);
    if(code == NULL) {
        printf("Error reading input file \"%s\"\n", inputFile);
        return 1;
    }
    // minify
    char *minified = minify(code);
    free(code);
    // compress
    char *compressed = compress(minified);
    free(minified);

    // translate
    char asmOutputFile[strlen(outputFile) + 9];
    snprintf(asmOutputFile, sizeof(asmOutputFile), "%s.bfc.asm", outputFile);

    int err = translate(compressed, asmOutputFile);
    free(compressed);

    if(err != SUCCESS) {
        printf("Encountered an error during translation:\n -> ");
        switch(err) {
            case ERROR_FILE_INACCESSIBLE:
                printf("Failed to open assembly output file '%s'\n", asmOutputFile);
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

    char objOutputFile[strlen(outputFile) + 6];
    sprintf(objOutputFile, "%s.bfc.o", outputFile);

    assemble(asmOutputFile, objOutputFile);
    link(objOutputFile, outputFile);

    if(!sFlag) {
        clean(asmOutputFile);
        clean(objOutputFile);
    }

    return 0;
}