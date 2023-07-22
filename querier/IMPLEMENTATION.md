# Ethan Hodess
## Implementation - querier


## Control flow

The Indexer is implemented in one file `indexer.c`, with three core functions.

### main

The `main` function simply calls `parseArgs` and `query`, then exits zero.

### parseArgs

Validate arguments and extract them into function parameters

* for `pageDirectory`, validate it exists and is produced by Crawler
* for `indexFilename`, ensure it can be read
* print an error to stderr and exit non-zero if anything is invalid

### query

Query the user and look at query. 

Pseudocode:

	open index file
    initialize index structure and load in from file
    read to EOF
        read in line from standard input
            check to see if the query is valid
                initialize the array for the words in the query
                determine the number of words in the query during extraction
                check to ensure operators are placed appropriately
                print the formatted query
                process the query
                find number of document matches
                check to see if any documents do not match
                create ranked document list, with appropriate memory allocation
                rank the documents, by iterating over counters
                print matching documents
            clean memory

### processQuery

Process the query as described in the requirements. 

Pseudocode:

	create two counters initially set to NULL
        stop early if AND sequence will fail
        loop over all words in query
            if read OR
            orSequence holds final result, merge in andSequence seen so far
            check to initialize orSequence if neccesary
            merge andSequence and orSequence
                clean memory
                reset shortCircuit
            AND sequence will fail because no results
                skip the words until see OR and shortCircuit is reset
            if read AND
                continue to next word, same as if AND was not there
            else read non-operator word
                think of stepping away to calculate AND sequence
                found counters for this word in index
                    this is the first word of AND sequence
                        copies match to andSequence
                    not first word in AND sequence
                        intersect this word's pages
                else no pages contain this query word
                    AND will fail for next word, so skip until see OR
                    because this word returned no pages, remove


## Function prototypes

More information in comments in querier.c.

```c
int main(const int argc, char* argv[]);
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
```

## Error handling and recovery

First, all command-line parameters are checked before any work begins. We also check for memory errors whenever needed.

We use defensive programming techniques to anticipate and catch potential errors as best as possible. 

## Testing 

### System testing

The script `testing.sh` invokes the querier several times with a variety of command-line arguments. First we test with various invalid arguments to make sure they get caught. Second, we pass valid pageDirectories and indexes from Crawler and Indexer. Last, we run with valgrind. We check that the querier runs as intended by observing the outputs.
