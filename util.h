#pragma once

#include <stdio.h>

long get_file_size(FILE *fptr);
char* read_file(char* filename, int compression, int verbose);