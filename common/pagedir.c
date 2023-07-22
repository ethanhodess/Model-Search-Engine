/* 
 * Ethan Hodess
 * pagedir.c - library of page directory functions
 * 
 * More information in pagedir.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/**************** Function Definitions ****************/
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_load(const char* filename);

/**************** File-Local Global Variables ****************/
/* None */

/**************** Local Types ****************/
/* None */

/**************** Global Types ****************/
/* None */

/**************** Local Functions ****************/
/* Not visible outside this file. */

/**************** Global Functions ****************/
/* Visible outside this file. */

bool
pagedir_init(const char* pageDirectory)
{
  // construct the pathname for the .crawler file in that directory
  char* filename = malloc(strlen(pageDirectory) + 10);
  
  // memory check
  if (filename == NULL) {
    fprintf(stderr, "Error: memory\n");
    return false;
  }
  
  sprintf(filename, "%s/.crawler", pageDirectory);
  FILE* fp = fopen(filename, "w");
  
  if (fp != NULL) {
    fclose(fp); 
    free(filename); 
    return true;
  } else {
    // on error, return false
    fprintf(stderr, "Error: file cannot be written\n");
    free(filename); 
    return false;
  }
}

void
pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
  // construct the pathname for the file in pageDirectory
  char* filename = malloc(strlen(pageDirectory) + 11);
  
  // memory check
  if (filename == NULL) {
    fprintf(stderr, "Error: memory\n");
    exit(1);
  }
  
  sprintf(filename, "%s/%d", pageDirectory, docID);
  FILE* fp = fopen(filename, "w");
  
  if (fp != NULL) {
    fprintf(fp, "%s\n", webpage_getURL(page)); // print URL
    fprintf(fp, "%d\n", webpage_getDepth(page)); // print depth
    fprintf(fp, "%s\n", webpage_getHTML(page)); // print the contents of the webpage
    
    fclose(fp); 
    free(filename); 
  } else {
     // on error, exit
    fprintf(stderr, "Error: file cannot be written\n");
    free(filename); 
    exit(1);
  }
}

bool
pagedir_validate(const char* pageDirectory)
{
  char* filename = malloc(strlen(pageDirectory) + 10);
  // memory check
  if (filename == NULL) {
    fprintf(stderr, "Error: memory\n");
    return false;
  }
  // set filename
  sprintf(filename, "%s/.crawler", pageDirectory);
  
  // open for reading
  FILE* fp = fopen(filename, "r");
  if (fp != NULL) {
    fclose(fp);
    free(filename);
    return true;

  } else {
    free (filename);
    return false;
  }
}

webpage_t*
pagedir_load(const char* filename)
{
  FILE* fp = fopen(filename, "r");
  if (fp != NULL) {
    // read file
    char* url = file_readLine(fp);
    char* depth = file_readLine(fp);
    char* html = file_readFile(fp);

    // convert depth to an int
    int depth_int;
    sscanf(depth, "%d", &depth_int);
    free(depth);
    
    // new webpage
    webpage_t* webpage = webpage_new(url, depth_int, html);

    fclose(fp);
    return webpage;

  } else {
    // can't be opened
    return NULL;
  }
}
