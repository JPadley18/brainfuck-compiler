#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

// Maximum number of nested loops allowed
#define MAX_LOOP_DEPTH 1024
// Size of buffer for reading numbers
#define NUM_BUF_SIZE 32

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
    write_instruction(fptr, "mov ebx, cells");
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
    // Write getchar procedure
    fprintf(fptr, "getchar:\n");
    write_instruction(fptr, "mov ecx, ebx");
    write_instruction(fptr, "mov eax, 3");
    write_instruction(fptr, "mov ebx, 0");
    write_instruction(fptr, "mov edx, 1");
    write_instruction(fptr, "int 0x80");
    write_instruction(fptr, "mov ebx, ecx");
    write_instruction(fptr, "ret");
    // Write putchar procedure
    fprintf(fptr, "putchar:\n");
    write_instruction(fptr, "mov ecx, ebx");
    write_instruction(fptr, "mov eax, 4");
    write_instruction(fptr, "mov ebx, 1");
    write_instruction(fptr, "mov edx, 1");
    write_instruction(fptr, "int 0x80");
    write_instruction(fptr, "mov ebx, ecx");
    write_instruction(fptr, "ret");
    // Write .bss section
    fprintf(fptr, "\nsection .bss\n");
    write_instruction(fptr, "cells: resb 30000");
}

/**
 * Translate one character of Brainfuck into assembly
 * 
 * @param c the character to translate
 * @param repeats the number of times to repeat the instruction
 * @param fptr the file to translate into
 * 
 * @return an error code if one occurred, else 0
*/
void translate_instruction(char c, int repeats, FILE *fptr) {
    char line[NUM_BUF_SIZE + 16];
    switch(c) {
        case '+':
            snprintf(line, sizeof(line), "add byte [ebx], %d", repeats);
            write_instruction(fptr, line);
            break;
        case '-':
            snprintf(line, sizeof(line), "sub byte [ebx], %d", repeats);
            write_instruction(fptr, line);
            break;
        case '<':
            snprintf(line, sizeof(line), "add ebx, %d", repeats);
            write_instruction(fptr, line);
            break;
        case '>':
            snprintf(line, sizeof(line), "add ebx, %d", repeats);
            write_instruction(fptr, line);
            break;
        case '.':
            write_instruction(fptr, "call putchar");
            break;
        case ',':
            write_instruction(fptr, "call getchar");
            break;
        default:
            return;
    }
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

    // Step through each instruction
    int numPtr = 0;
    char numBuf[NUM_BUF_SIZE];
    for(int i = 0; code[i] != '\0'; i++) {
        // If reading a number, read whole number
        int repeats = 1;
        if(code[i] >= '0' && code[i] <= '9') {
            numBuf[numPtr++] = code[i];
            continue;
        } else if(numPtr > 0) {
            // If a number is stored, use it as repeats for next instruction
            numBuf[numPtr] = 0;
            repeats = atoi(numBuf);
            numPtr = 0;
        }
        translate_instruction(code[i], repeats, fptr);
    }

    write_footer(fptr);
    fclose(fptr);
    return SUCCESS;
}