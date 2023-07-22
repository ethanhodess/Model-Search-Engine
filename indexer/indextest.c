/*
 * Ethan Hodess
 * indextest.c
 * unit test for index module
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <index.h>
#include <file.h>

int
main(int argc, char *argv[]) 
{
  // check arguments
  if (argc != 3) {
    fprintf(stderr, "Usage: ./indextest [oldIndexFilename] [newIndexFilename]\n");
    exit(1);
  }

  // creating the 'read' file
  FILE* old_fp = fopen(argv[1], "r");

  if (old_fp == NULL) {
    fprintf(stderr, "Error: cannot open file");
    exit(1);
  }

  int num_slots = file_numLines(old_fp);
  fclose(old_fp);

  index_t* index = index_new(num_slots);

  FILE* new_fp = fopen(argv[2], "w");
  if (new_fp == NULL) {
    fprintf(stderr, "Error: cannot open file");
    exit(1);
  }

  fclose(new_fp);

  index_load(index, argv[1]);
  index_save(index, argv[2]);
  index_delete(index);

  exit(0);
}
