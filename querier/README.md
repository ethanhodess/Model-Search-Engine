# CS50 TSE - Ethan Hodess

### querier

The _querier_ module is the third part of the Tiny Search Engine.

The TSE querier is a standalone program that reads index files and responds to queries from stdin.

The querier supports full functionality. The program should print the set of documents that contain all the words in the query. It also treats 'and' with higher precedence than 'or.' It also prints the document set in decreasing order by score.


### Files

* `Makefile` - compilation procedure
* `querier.c` - main code
* `README.md` - extra info about the module
* `testing.sh` - shell testing script
* `testing.out` - result of `make test &> testing.out`
* `IMPLEMENTATION.md` - a description of my implementation
* `DESIGN.md` - description of my design

### Compilation

To compile, `make` 