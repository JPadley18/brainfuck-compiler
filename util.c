#include <stdio.h>
#include <stdlib.h>

/**
 * Get the size in bytes of the given opened file
 * 
 * @return the size of the file in bytes
*/
long get_file_size(FILE *fptr) {
    // Seek to the end of the file
    fseek(fptr, 0, SEEK_END);
    // Use current pointer position as file size
    long size = ftell(fptr);
    rewind(fptr);
    return size;
}

/**
 * Reads the given file into memory.
 * 
 * @param filename char pointer to the filename string
 * 
 * @return a pointer to the beginning of the data, NULL if the file cannot be read.
*/
char* read_file(char* filename) {
    FILE *fptr = fopen(filename, "r");
    // Return null if the file cant be opened
    if(fptr == NULL) {
        return NULL;
    }
    // Allocate a buffer to store the file contents + a null char at the end
    long size = get_file_size(fptr);
    char *buffer = malloc(size + 1);
    size_t n = fread(buffer, size, 1, fptr);
    if(n < 1) {
        fclose(fptr);
        return NULL;
    }
    buffer[size] = 0;
    fclose(fptr);

    return buffer;
}