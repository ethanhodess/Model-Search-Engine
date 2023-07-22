/* 
 * Ethan Hodess
 * index.c - library to read and write index files
 * more information in index.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"

/**************** Local Types ****************/
typedef struct index {
    hashtable_t* hashtable;
} index_t;

/**************** Function Prototypes ****************/
index_t* index_new(const int num_slots);
bool index_insert(index_t* index, const char* key, void* item);
void* index_find(index_t* index, const char* key);
void index_delete(index_t* index);
void index_save(index_t* index, const char* filePath);
void index_load(index_t* index, const char* filePath);

void index_itemdelete(void* item);
void index_itemfunc(void* arg, const char* key, void* item);
void counters_itemfunc(void* arg, const int key, const int number);

/**************** File-Local Global Variables ****************/
/* None */

/**************** Global Types ****************/
/* None */

/**************** Local Functions ****************/
/* Not visible outside this file. */

/**************** Global Functions ****************/
/* Visible outside this file. */

index_t*
index_new(const int num_slots)
{
  if (num_slots > 0) {
    index_t* index = malloc(sizeof(index_t));
    
    if (index == NULL) {
      return NULL; 
    } else {
      // initialize contents of index
      index->hashtable = hashtable_new(num_slots);
      return index;
    }
  }
  return NULL;
}

bool
index_insert(index_t* index, const char* key, void* item)
{
  // check parameters
  if (index != NULL && key != NULL && item != NULL) {
    return hashtable_insert(index->hashtable, key, item);
  } else {
    return false;
  }
}

void*
index_find(index_t* index, const char* key)
{
  // check parameters
  if (index != NULL && key != NULL) {
    return hashtable_find(index->hashtable, key);
  } else {
    return NULL;
  }
}

void
index_delete(index_t* index)
{
  // check that index is valid
  if (index != NULL) {
    hashtable_delete(index->hashtable, index_itemdelete);
    free(index);
  }
}

void
index_itemdelete(void* item)
{
  counters_delete(item);
}

void
index_save(index_t* index, const char* filePath)
{
  // open the file for writing
  FILE* fp = fopen(filePath, "w");

  // check that the file is valid
  if (fp != NULL) {
    hashtable_iterate(index->hashtable, fp, index_itemfunc);
    fclose(fp);
  } else {
    fprintf(stderr, "error: file cannot be written\n");
    exit(1);
  }
}

void
index_itemfunc(void* arg, const char* key, void* item)
{
  FILE* fp = arg;
  fprintf(fp, "%s ", key);

  // iterate through the counters for the word
  counters_t* counters = item;
  counters_iterate(counters, fp, counters_itemfunc);

  fprintf(fp, "\n");
}

void
counters_itemfunc(void* arg, const int key, const int number)
{
  FILE* fp = arg;
  fprintf(fp, "%d %d ", key, number);
}

void
index_load(index_t* index, const char* filePath)
{
  // open the file for reading
  FILE* fp = fopen(filePath, "r");

  // check that the file is valid
  if (fp != NULL) {
    char* word = NULL;
    int docID = 0;
    int count = 0;

    bool firstPair = true;

    while ((word = file_readWord(fp)) != NULL) {
      while (fscanf(fp, "%d %d ", &docID, &count) == 2) {
        if (firstPair) {
          // create a new counters and set its value
          counters_t* counters = counters_new();
          counters_set(counters, docID, count);

          index_insert(index, word, counters);
          firstPair = false;

        } else {
          // not the first pair
          counters_set(index_find(index, word), docID, count);
        }
      }
      // reset boolean
      firstPair = true;
      free(word);
    }
    fclose(fp);

  } else {
    fprintf(stderr, "Error: file cannot be read\n");
    exit(1);
  }
}

