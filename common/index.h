/* 
 * Ethan Hodess
 * index.h - header for index.c
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"

/**************** Global Types ****************/
typedef struct index index_t;

/**************** Functions ****************/

/**************** index_new ****************/
/* Create a new (empty) index structure.
 *
 * Caller provides:
 *   number of slots for the index (must be > 0)
 * We return:
 *   pointer to the new index structure; NULL if error (out of memory).
 * We guarantee:
 *   index is initialized empty.
 * Caller is responsible for:
 *   later calling index_delete().
 */
index_t* index_new(const int num_slots);

/**************** index_insert ****************/
/* Insert an item (with string key) into the given index structure.
 *
 * Caller provides:
 *   pointer to index, string for key, pointer to item.
 * We return:
 *   false if any parameter is null, key exists in index, or error;
 *   true iff new item was inserted.
 */
bool index_insert(index_t* index, const char* key, void* item);

/**************** index_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   pointer to index, string for key.
 * We return:
 *   pointer to the item corresponding to the given key, if found;
 *   null if index or key is NULL, or if key is not found.
 */
void* index_find(index_t* index, const char* key);

/**************** index_delete ****************/
/* Delete index structure.
 *
 * Caller provides:
 *   index pointer
 * We do:
 *   nothing, if index is NULL.
 *   otherwise, free all the string keys and the index structure itself.
 */
void index_delete(index_t* index);

/**************** index_save ****************/
/* Saves an index structure to the given file path.
 *
 * Caller provides:
 *   index pointer
 *   file path
 * We do:
 *   nothing, if index or filePath is null.
 *   otherwise, save the index structure.
 */
void index_save(index_t* index, const char* filePath);

/**************** index_load ****************/
/* Loads an index structure from the given file path.
 *
 * Caller provides:
 *   index pointer
 *   file path
 * We do:
 *   nothing, if index or filePath is null.
 *   otherwise, load the index structure.
 */
void index_load(index_t* index, const char* filePath);

#endif // __INDEX_H
