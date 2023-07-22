/* 
 * Ethan Hodess
 * pagedir.h - header file for pagedir
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/**************** Functions ****************/

/**************** pagedir_init ****************/
/* Check that the directory can be initialized and is valid
 *
 * Caller provides:
 *   string for pageDirectory
 * We return:
 *   false if can't create .crawler file
 *   true, otherwise
 */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* Save and write the webpage to the pageDirectory with the appropriate ID
 *
 * Caller provides:
 *   webpage pointer, string for pageDirectory, and ID
 * We do:
 *   save the URL, ID, and HTML contents to the page directory
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_validate ****************/
/* Checks if the page directory is a Crawler-produced directory
 *
 * Caller provides:
 *   pageDirectory string
 * We return:
 *   false if the page directory either does not exist or does not contain .crawler
 *   true, otherwise
 */
bool pagedir_validate(const char* pageDirectory);

/**************** pagedir_load ****************/
/* Loads a page from a file in the directory
 *
 * Caller provides:
 *   filename string
 * We return:
 *   a webpage type with the url, depth, and html
 */
webpage_t* pagedir_load(const char* filename);


#endif // __PAGEDIR_H

