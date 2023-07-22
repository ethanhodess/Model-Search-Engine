# CS50 TSE - Ethan Hodess

### indexer

The _indexer_ module is the second part of the Tiny Search Engine.

The TSE indexer is a standalone program that reads crawler file outputs, builds an index, and writes the index to a file:

`indexer.c` runs the index methods to build the index from the crawler directory, and it then prints an indexer output directory to a given filename

`indextest.c` takes an index file, loads it into the index struct, and then prints it out to another file. 

### Files

* `Makefile` - compilation procedure
* `indexer.c` - main code
* `README.md` - extra info about the module
* `testing.sh` - shell testing script
* `testing.out` - result of `make test &> testing.out`
* `IMPLEMENTATION.md` - a description of my implementation

### Compilation

To compile, `make all` 
