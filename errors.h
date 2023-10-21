#pragma once

// Loop is opened but never closed
#define ERROR_UNCLOSED_LOOP 0
// Loop is closed but never opened
#define ERROR_UNMATCHED_LOOP 1
// Max loop depth exceeded
#define ERROR_STACK_OVERFLOW 2