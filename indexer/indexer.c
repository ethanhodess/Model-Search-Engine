/*
 * Ethan Hodess
 * indexer.c - indexer for tiny search engine
 *
 * The TSE indexer is a standalone program which reads files from a directory previously filled by the crawler. It
 * builds an index and writes it to a file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <webpage.h>
#include <index.h>
#include <pagedir.h>
#include <word.h>

// Function prototypes
static void indexBuild(char* pageDirectory, char* indexFilename);
static void indexPage(webpage_t* webpage, int docID, index_t* index);

/*
 * Parses arguments and initializes modules
 */
int
main(const int argc, char* argv[])
{
  // check number of arguments
  if (argc != 3){
    fprintf(stderr, "Usage: ./indexer [pageDirectory] [indexFilename]\n");
    exit(1);
  }

  // check for null arguments
  if (argv[0] == NULL || argv[1] == NULL || argv[2] == NULL){
    fprintf(stderr, "Error: null arguments\n");
    exit(1);
  }

  if (!pagedir_validate(argv[1])){
    fprintf(stderr, "Error: page directory is not valid\n");
    exit(1);
  }

  indexBuild(argv[1], argv[2]);
  exit(0);
}

/*
 * Builds an index from webpage files
 */
static void
indexBuild(char* pageDirectory, char* indexFilename)
{
  //creates a new 'index' object and webpage
  index_t* index = index_new(800);
  webpage_t* webpage;

  //loops over document ID numbers, counting from 1
  int docID = 1;

  char* filename = malloc(strlen(pageDirectory) + 10);
  //memory check
  if (filename == NULL){
    fprintf(stderr, "Error: memory\n");
    exit(1);
  }

  //set filename
  sprintf(filename, "%s/%d", pageDirectory, docID);

  //loads a webpage from the document file 'pageDirectory/id'
  
  while ((webpage = pagedir_load(filename)) != NULL) {
    // if successful,
    //passes the webpage and docID to indexPage
    indexPage(webpage, docID, index);
    

    docID += 1;
    //update filename
    sprintf(filename, "%s/%d", pageDirectory, docID);

    webpage_delete(webpage);
  }
  
  index_save(index, indexFilename);
  index_delete(index);
  free(filename);
}

/*
 * Scans a webpage document to add its words to the index
 */
static void
indexPage(webpage_t* webpage, int docID, index_t* index)
{
  int pos = 0;
  char* word;

  while ((word = webpage_getNextWord(webpage, &pos)) != NULL) {
    // skips trivial words (less than lenth 3)
    if (strlen(word) > 2) {
      // normalizes the word (converts to lower case)
      normalizeWord(word);

      // looks up the word in the index
      if (index_find(index, word) == NULL) {
        //if first occurrence
        counters_t* counter = counters_new();
        counters_add(counter, docID);

        // adding the word to the index if needed
        index_insert(index, word, counter);
      } else {
        // not first occurrence

        // increments the count of occurrences of this word in this docID
        counters_add(index_find(index, word), docID);
      }
    }
    free(word);
  }
}
