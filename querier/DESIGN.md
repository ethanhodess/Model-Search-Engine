# Ethan Hodess
## Design - querier

### User interface

The querier's only interface with the user is arguments on the command line followed by input from stdin. To call querier, it must always have two arguments.

```
querier pageDirectory indexFilename
```

### Inputs and outputs

Input: The first inputs are command line parameters. The second inputs are queries in the form of words submitted by the user. 

Output: We output to stdout the result of the search query, formatted according to the requirements.

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *parseArgs*, which parses arguments and initializes other modules;
 2. *query*, which prompts the user for a query and tokenizes it;
 3. *validQuery*, which validates the query;
 4. *extractWords*, which parses/tokenizes the input;
 5. *validOperators*, which validates the format of the query.
 6. *processQuery*, which creates a counter set that is used to print out to stdout.
 7. *countMatches*, which counts the number of matches.
 8. *sortMatches*, which sorts the documents by score.


### Pseudo code for logic/algorithmic flow

The indexer will run as follows:

    parse the command line, validate parameters, initialize other modules
    call query

where *indexBuild:*

      initialize index structure and load in from file
      read until EOF
        read in line from standard input
        check the query
          initialize the array for the words in the query
          determine the number of words in the query during extraction
          check operators
            print the formatted query
            process the query
            find number of document matches
            check if any documents do not match
            create document list
            rank the documents, by iterating over counters
            print matching documents

where *processQuery:*

    create two counters initially set to NULL
      stop early if AND sequence will fail
        loop over all words in query
        handle AND and OR appropriately
        handle non-operator words

### Major data structures

We use the *counters* data structure to hold *(docID, score)* pairs for the documents. 
We also define a *document* struct to hold the *docID*, *score*, and *URL* for a given document.
We define *countersTuple* to store two counter sets. 
The key data structure is the *counters*, which holds the *(docID, score)* pairs for the documents identified by the query.
A *document* struct is used to hold the *docID*, *score*, and *URL* for a given document, based on a query. 
A *all* struct is used to keep a document, score, and pageDirectory. 

### Testing plan

The querier is tested by running various queries on different directories to check the output. 

1. Test `querier` with various invalid arguments.
	2. no arguments
	3. one argument
	4. too many arguments
	5. invalid `pageDirectory` (non-existent path)
	5. invalid `pageDirectory` (not a crawler directory)
	6. invalid `indexFile` (non-existent path)
	7. invalid `indexFile` (read-only directory)
	7. invalid `indexFile` (existing, read-only file)
2. Run querier on other different pageDirectories
3. Run valgrind on  querier