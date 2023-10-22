#pragma once

// No error
#define SUCCESS 0
// Loop is opened but never closed
#define ERROR_UNCLOSED_LOOP 1
// Loop is closed but never opened
#define ERROR_UNMATCHED_LOOP 2
// Max loop depth exceeded
#define ERROR_STACK_OVERFLOW 3
// Code file could not be opened
#define ERROR_FILE_INACCESSIBLE 4