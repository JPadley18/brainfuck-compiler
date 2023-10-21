#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Compiles a regex with given pattern
 * 
 * @param pattern the regex pattern to compile
 * @param regex a regex_t to compile the regex with
*/
void compile_regex(char *pattern, regex_t *regex) {
    int err = regcomp(regex, pattern, 0);
    if(err) {
        char errBuf[50];
        regerror(err, regex, errBuf, 50);
        printf("Error compiling regex with pattern \"%s\": %s\n", pattern, errBuf);
    }
}

/**
 * Removes any chars that do not match the regex from the string
 * 
 * @param source a char pointer to the source string
 * @param dest a char pointer to a destination buffer
 * @param pattern a regex pattern to compare against
*/
void remove_non_matches(char *source, char *dest, char *pattern) {
    regex_t regex;
    compile_regex(pattern, &regex);
    // Run regexec
    int sourceOffset = 0;
    int destOffset = 0;
    regmatch_t matches[regex.re_nsub + 1];
    while(1) {
        int err = regexec(&regex, source + sourceOffset, 1, matches, 0);
        if(err != 0) {
            // Stop regex - out of matches
            break;
        }
        int matchLength = matches[0].rm_eo - matches[0].rm_so;
        memcpy(dest + destOffset, source + sourceOffset + matches[0].rm_so, matchLength);

        sourceOffset += matches[0].rm_eo;
        destOffset += matchLength;
    }
    dest[destOffset] = 0;

    regfree(&regex);
}

/**
 * Remove non-brainfuck chars from given string
 * 
 * @param source char pointer to the null-terminated code source
 * 
 * @return char pointer to minified code
*/
char *minify(char *source) {
    char *dest = malloc(strlen(source));
    remove_non_matches(source, dest, "[][><,.+-]\\{1,\\}");
    return dest;
}

/**
 * Use run-length encoding to turn repeated sequences of chars into the form [count][char]
 * 
 * @param source char pointer to the source string
 * 
 * @return char pointer to compressed code
*/
char *compress(char *source) {
    char c;
    int destPtr = 0;
    char currentRepeat = -1;
    int repeats = 1;
    int i = 0;
    char *dest = malloc(strlen(source));
    while((c = source[i++]) != 0) {
        // If the current repeated char changes or we reach the end of the string
        if(c != currentRepeat) {
            if(repeats > 1) {
                char str[11];
                snprintf(str, 11, "%d", repeats);
                // Copy numeric string to destination
                int len = strlen(str);
                memcpy(dest + destPtr, str, len);
                destPtr += len;
            }
            if(currentRepeat != -1) {
                dest[destPtr++] = currentRepeat;
            }
            repeats = 1;
            if(c == ']' || c == '[') {
                currentRepeat = -1;
                dest[destPtr++] = c;
            } else {
                currentRepeat = c;
            }
        } else {
            repeats++;
        }
    }

    if(currentRepeat != -1) {
        dest[destPtr] = currentRepeat;
        dest[destPtr + 1] = 0;
    } else {
        dest[destPtr] = 0;
    }

    return dest;
}