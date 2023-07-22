/* 
 * Ethan Hodess
 * querier.c - querier for tiny search engine
 *
 * The TSE querier reads index files from the TSE indexer and uses them to respond to user queries.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <index.h>
#include <file.h>
#include <word.h>
#include <counters.h>
#include <pagedir.h>


/************* Function Prototypes ********************/
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
static void query(char* pageDirectory, char* indexFilename);
static bool validQuery(char* line);
static int extractWords(char* line, char** words);
static bool validOperators(char** words, int count);
static bool isOperator (char* word);

static counters_t* processQuery(index_t* index, char* pageDirectory, char** words, int count);

static void unionSeq(counters_t* orSequence, counters_t* otherSequence);
static void unionHelper(void* arg, const int key, int count);
static void intersectSeq(counters_t* orSequence, counters_t* otherSequence);
static void intersectHelper(void* arg, const int key, int count);

static void countMatches(void* arg, const int docID, int score);
static void sortMatches(void *arg, const int docID, int score);

static char* getDocumentURL(char* pageDirectory, int docID);


/**************** File-Local Global Variables ****************/
/* None */

/**************** Local Types ****************/
typedef struct countersTuple {
  counters_t* counters1;
  counters_t* counters2;
} countersTuple_t;

typedef struct document {
  int docID;
  int score;
  char* URL;
} document_t;

typedef struct all {
  document_t** documents;
  int matches;
  char* pageDirectory;
} all_t;

/**************** Global Types ****************/
/* None */

/**************** Global Functions ****************/
/* Visible outside this file. */

/**************** Local Functions ****************/
/* Not visible outside this file. */

/**************** main ****************/
/* Description: Calls 'parseArgs' and 'queryUser',
 * then exits zero.
 */
int
main(const int argc, char* argv[])
{
  parseArgs(argc, argv, &argv[1], &argv[2]);
  query(argv[1], argv[2]);
  exit(0);
}

/**************** parseArgs ****************/
/* 
 * Check that arguments are valid and extract them into function parameters
 */
static void
parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename)
{
  // check number of arguments 
  if (argc != 3) {
    fprintf(stderr, "Usage: ./querier [pageDirectory] [indexFilename]\n");
    exit(1);
  }
  
  // check for null arguments
  if (argv[0] == NULL || argv[1] == NULL || argv[2] == NULL) {
    fprintf(stderr, "Usage: ./querier [pageDirectory] [indexFilename]\n");
    exit(1);
  }

  // check that page directory exists and make by crawler
  if (!pagedir_validate(*pageDirectory)) {
    fprintf(stderr, "Error: page directory is invalid\n");
    exit(1);
  }

  // check that the index filename is readable
  FILE* fp = fopen(*indexFilename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: index filename is not readable\n");
    exit(1);

  } else {
    fclose(fp);
  }
}

/**************** query ****************/
/* 
 * Main query code gets a query from the user and tokenizes the result. 
 */
static void
query(char* pageDirectory, char* indexFilename)
{
  // open index file
  FILE* fp = fopen(indexFilename, "r");
  int slots = file_numLines(fp); 
  fclose(fp); 

  // initialize index structure and load from file
  index_t* index = index_new(slots);
  index_load(index, indexFilename);

  printf("Query? ");

  char* line;

  while (!feof(stdin)) {
    while ((line = file_readLine(stdin)) != NULL) {
      if (validQuery(line)) {
        // initialize the array for query words
        char** words = malloc((strlen(line) + 1) * sizeof(char*));

        // get number of query words
        int count = extractWords(line, words);

        // verify operators
        if (count != 0 && words != NULL && validOperators(words, count)) {
          printf("Query: ");

          // print the new formatted query
          for (int i = 0; i < count; i++) {
            printf("%s ", words[i]);
          }
          printf("\n");

          counters_t* result = processQuery(index, pageDirectory, words, count);

          // iterate to get number of matches
          int matches = 0;
          counters_iterate(result, &matches, countMatches);

          if (matches == 0) {
            printf("No matches\n");

          } else {
            // if we have matches

            document_t* ranked_list[matches];
            memset(ranked_list, '\0', sizeof(ranked_list));

            all_t* all = malloc(sizeof(all_t));
            all->documents = ranked_list;
            all->matches = matches;
            all->pageDirectory = pageDirectory; 

            counters_iterate(result, all, sortMatches);
            printf("Query matches with %d documents (ranked):\n", matches);

            for (int i = 0; i < matches; i++) {
                document_t* doc = ranked_list[i]; // extract document from list
                printf("score %3d doc: %3d: %s\n", doc->score, doc->docID, doc->URL);

                free(doc->URL);
                free(doc);
            }
            free(all);
          }
          counters_delete(result);
          printf("-----------------------------------------------\n");
        }

        free(words);
      }

      printf("Query? ");

      free(line);
    }
  }  
  printf("\n");
  index_delete(index);
}

/**************** validQuery ****************/
/* 
 * Checks the query for invalid characters and returns accordingly
 */
static bool
validQuery(char* line) {
  for (int i = 0; i < strlen(line); i++) {
    if (!isalpha(line[i]) && !isspace(line[i])) {
      fprintf(stderr, "Error: invalid character '%c' in query\n", line[i]);
      return false;
    }
  }
  return true;
}

/**************** extractWords ****************/
/* 
 * Extracts the words from the query using the sliding method
 */
static int
extractWords(char* line, char** words) {
  int i = 0;
  int size = strlen(line);
  int count = 0;

  // pointers for sliding
  int c;
  char* word;
  char* rest;

  while (i < size) {
    // get the character at a  point
	c = line[i];

    // check character
	if (isalpha(c)) {
      // update the memory position
	  word = &line[i];

      // add the normalized word to the array
	  words[count] = normalizeWord(word);
	  count += 1;

      // slide across memory
      int j = i;
      int v = line[j];
      while (isalpha(v)) {
        v = line[j];
        j += 1;
      }

      // set the end point
	  rest = &line[j - 1];
	  *rest = '\0';

      i = j - 1;
	}
    i += 1;
  }		
  return count;
}

/**************** validOperators ****************/
/*
 * Checks that operators don't start or end query and are not adjacent
 */
bool
validOperators(char** words, int count)
{
  // check for empty query
  if (words[0] == NULL || strcmp(words[0], " ") == 0 || strcmp(words[0], "") == 0) {
    return false;
  }

  // check for operator at beginning of query
  if (isOperator(words[0])) {
    fprintf(stderr, "Error: '%s' cannot be first\n", words[0]);
    return false;
  }

  // check for operator at end of query
  if (isOperator(words[count - 1])) {
    fprintf(stderr, "Error: '%s' cannot be last\n", words[count - 1]);
    return false;
  }

  // check for consecutive operators
  for (int i = 0; i < count - 2; i++) {
    if (isOperator(words[i]) && isOperator(words[i + 1])) {
      fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", words[i], words[i + 1]);
      return false;
    }
  }

  return true;
}

/**************** isOperator ****************/
/*
 * Checks if a word is an operator
 */
static bool
isOperator (char* word)
{
  if ((strcmp(word, "and") == 0) || (strcmp(word, "or") == 0)) {
    return true;
  } else {
    return false;
  }
}


/**************** processQuery ****************/
/* 
 * Reads the query and processes it into counter set
 * Adapted from pseudocode on Professor Pierson's class site
 */
static counters_t*
processQuery(index_t* index, char* pageDirectory, char** words, int count)
{
  counters_t* orSequence = NULL;
  counters_t* andSequence = NULL;

  bool shortCircuit = false;

  // loop over all words in query
  for (int i = 0; i < count; i++) {

    // if read OR
    if (strcmp(words[i], "or") == 0) {

      // initialize orSequence if necessary
      if (orSequence == NULL) {
        orSequence = counters_new();
      }
      
      // merge orSequence and andSequence
      if (andSequence != NULL) {
        unionSeq(orSequence, andSequence);        
        counters_delete(andSequence);
        andSequence = NULL;
      }

      shortCircuit = false;

      continue;
    }

    // sequence will fail
    if (shortCircuit) {
      // skip until we see OR
      continue;
    }

    // if read AND
    if (strcmp(words[i], "and") == 0) {
      continue;  // continue - implicit AND between words
    }

    // if read a word (not OR or AND)
    else {
      // find counters in index
      counters_t* match = index_find(index, words[i]);

      if (match != NULL) {
        // first word in AND sequence
        if (andSequence == NULL) {
          andSequence = counters_new();
          unionSeq(andSequence, match);
        } else {
          // not first word in AND sequence
          intersectSeq(andSequence, match);
        }
      } else {
        // AND will fail so skip
        shortCircuit = true;
        
        if (andSequence != NULL) {
          // no pages
          counters_delete(andSequence);
          andSequence = NULL;
        }
      }
    }
  }

  if (orSequence == NULL) {
    orSequence = counters_new();
  }
  if (andSequence != NULL) {
    unionSeq(orSequence, andSequence);
    counters_delete(andSequence);
    andSequence = NULL;
  }

  return orSequence;
}

/**************** unionSeq ****************/
/* 
 * Union method for two counter sets which adds counts for matching keys and stores results in orSequence
 */
static void
unionSeq(counters_t* orSequence, counters_t* otherSequence)
{
  counters_iterate(otherSequence, orSequence, unionHelper);
}

/**************** unionHelper ****************/
/* 
 * Helper method updates counterset
 */
static void
unionHelper(void* arg, const int key, int count)
{
  int wordCount = counters_get(arg, key); 
  counters_set(arg, key, count + wordCount);
}

/**************** intersectSeq ****************/
/* 
 * Intersect method for two counter sets which takes counts of matching keys and stores in orSequence
 */
static void
intersectSeq(counters_t* counters1, counters_t* counters2)
{
  countersTuple_t* args = malloc(sizeof(countersTuple_t));
  args->counters1 = counters1;
  args->counters2 = counters2;

  counters_iterate(counters1, args, intersectHelper);
  free(args);
}

/**************** intersectHelper ****************/
/* 
 * Helper method updates counters
 */
static void
intersectHelper(void* arg, const int key, int count)
{
  countersTuple_t* args = arg;

  int otherCount = counters_get(args->counters2, key);
  int minValue = count < otherCount ? count : otherCount;

  counters_set(args->counters1, key, minValue);
}

/**************** countMatches ****************/
/* 
 * Counts the number of document matches
 */
static void
countMatches(void* arg, const int docID, int score)
{
  int* matches = arg;

  if (matches != NULL && score != 0) {
    (*matches) += 1;
  }
}

/**************** sortMatches ****************/
/* 
 * Sorts the matches based on score
 */
static void
sortMatches(void* arg, const int docID, int score)
{
  // skip if no matches
  if (score == 0) {
    return;
  }

  all_t* all = arg;
  document_t** documents = all->documents;
  int matches = all->matches;
  char* pageDirectory = all->pageDirectory;

  char* URL = getDocumentURL(pageDirectory, docID);

  document_t* thisDocument = malloc(sizeof(document_t));
  thisDocument->docID = docID;
  thisDocument->score = score;
  thisDocument->URL = URL;

  // use insertion sort
  for (int i = 0; i < matches; i++) {
    document_t* document = documents[i];
    if (document == NULL) {
      documents[i] = thisDocument;
      return;
    }

    if (document->score < score) {
      for (int j = matches - 2; j >= i; j--) {
        documents[j + 1] = documents[j];
      }
      
      documents[i] = thisDocument;
      return;
    } 
  }
}

/**************** getDocumentURL ****************/
/* 
 * Extracts the URL for a query result for output
 */
static char*
getDocumentURL(char* pageDirectory, int docID)
{
  char* filename = malloc(strlen(pageDirectory) + 10);

  // memory check
  if (filename == NULL) {
    fprintf(stderr, "Error: memory\n");
    return NULL;
  }

  // set filename
  sprintf(filename, "%s/%d", pageDirectory, docID);

  FILE* fp = fopen(filename, "r");
  free(filename);

  // check that file can be opened
  if (fp != NULL) {
    char* URL = file_readLine(fp);
    fclose(fp); 
    return URL;

  } else {
    fprintf(stderr, "Error: cannot open file\n");
    return NULL;
  }
}