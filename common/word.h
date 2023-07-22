/* 
 * Ethan Hodess
 * word.h - header for word.c
 * library of word related functions
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/**************** Functions ****************/

/**************** normalizeWord ****************/
/* Returns a given word all lowercase
 *
 * Caller provides:
 *   string for word
 * We return:
 *   pointer to the lowercase version
 *   null if word is null
 */
char* normalizeWord(char* word);

#endif // __WORD_H

