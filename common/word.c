/* 
 * Ethan Hodess
 * word.c - library of word functions
 * more information in word.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/**************** Function Definitions ****************/
char* normalizeWord(char* word);

/**************** File-Local Global Variables ****************/
/* None */

/**************** Global Types ****************/
/* None */

/**************** Local Functions ****************/
/* Not visible outside this file. */

/**************** Global Functions ****************/
/* Visible outside this file. */

char*
normalizeWord(char* word) {
  if (word != NULL) {
    // loop through the characters of the string
    for (int i = 0; i < strlen(word); i++) {
      // make the character lowercase
      word[i] = tolower(word[i]);
    }
  }
  return word;
}

