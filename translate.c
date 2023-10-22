#include <stdio.h>
#include "errors.h"

// Maximum number of nested loops allowed
#define MAX_LOOP_DEPTH 1024

/**
 * Writes a single assembly instruction to the given file
 * 
 * @param fptr the file to write to
 * @param instruction the instruction line to write
*/
void write_instruction(FILE *fptr, char *instruction) {
    fprintf(fptr, "    %s\n", instruction);
}

/**
 * Writes the initial assembly header to the given file
 * 
 * @param fptr the file descriptor to write to
*/
void write_header(FILE *fptr) {
    fprintf(fptr, "global _start\n\nsection .text\n\n_start:\n");
}

/**
 * Writes the final assembly footer to the given file
 * 
 * @param fptr the file descriptor to write to
*/
void write_footer(FILE *fptr) {
    // Write the exit block
    write_instruction(fptr, "mov eax, 1");
    write_instruction(fptr, "mov ebx, 0");
    write_instruction(fptr, "int 0x80");
    // Write .bss section
    fprintf(fptr, "\nsection .bss\n");
    write_instruction(fptr, "cells: resb 30000");
}

/**
 * Translate the given Brainfuck code into assembly and write to the given file
 * Supports run-length encoded Brainfuck
 * 
 * @param code pointer to the beginning of the code string
 * @param file the filename to write to
 * 
 * @return an error code if one occurred, else 0
*/
int translate(char *code, char *file) {
    FILE *fptr = fopen(file, "w");
    if(fptr == NULL) {
        return ERROR_FILE_INACCESSIBLE;
    }
    write_header(fptr);

    // Assemble here

    write_footer(fptr);
    fclose(fptr);
    return SUCCESS;
}