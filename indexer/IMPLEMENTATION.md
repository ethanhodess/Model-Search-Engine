# Lab 5 - Ethan Hodess
# TSE Indexer - Implementation 

## Data structures 

We use an `index` as defined in `index.h`. It is a wrapper for `hashtable`. We also use 'webpage' to collect the url, depth, and html. The 'index' is then used to store the result.

The modules are developed in the 'common' library.

## Control flow

Indexer is implemented in one file `indexer.c` with three functions.

### main

The `main` function parses arguments, validates parameters, and initializes modules, then calls `indexBuild`. For `pageDirectory`, it validates that it exists and is produced by Crawler.

### indexBuild

Build an index from the `pageDirectory` and save the index to the `indexFilename`.

Pseudocode:


      creates a new 'index' object
      loops over document ID numbers, counting from 1
        loads a webpage from the document file 'pageDirectory/id'
        if successful, 
          passes the webpage and docID to indexPage

### indexPage

Extracts the words from the webpage, checks if they are of length >= 3, and normalizes them. Then, inserts into the index using a counter set.

Pseudocode:


     steps through each word of the webpage,
       skips trivial words (less than length 3),
       normalizes the word (converts to lower case),
       looks up the word in the index,
         adding the word to the index if needed
       increments the count of occurrences of this word in this docID


## Function prototypes

### indexer

See indexer.h and indexer.c for more information. 

```c
int main(const int argc, char* argv[]);
static void indexBuild(char* pageDirectory, char* indexFilename);
static void indexPage(webpage_t* webpage, int docID, index_t* index);
```

### index

See index.h for more information

```c
index_t* index_new(const int num_slots);
bool index_insert(index_t* index, const char* key, void* item);
void* index_find(index_t* index, const char* key);
void index_delete(index_t* index);
void index_save(index_t* index, const char* filePath);
void index_load(index_t* index, const char* filePath);
void index_itemdelete(void* item);
void index_itemfunc(void* arg, const char* key, void* item);
void counters_itemfunc(void* arg, const int key, const int number);
```

### pagedir

See pagedir.h for more information.

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_load(const char* filename);
```

### word

See word.h for more information.

```c
char* normalizeWord(char* word);
```

## Testing plan

### Unit testing

We write `indextest` for unit testing. It reads an index file into an index data structure, then writes it to a new index file.

### System testing

We write `testing.sh` which invokes the indexer several times with a variety of arguments. First we use erroneous arguments to test possible mistakes that can be made. Second, it tests prior Crawler output. Next we run with valgrind.

