# Ethan Hodess
# Makefile for indexer

L = ../libcs50
C = ../common

OBJS = indexer.o indextest.o
LIB = $C/common.a $L/libcs50.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# By default, the Makefile builds both 'indexer' and 'indextest' by default (phony top-most target).
all: indexer indextest

indexer: indexer.o $(LIB)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@

indextest: indextest.o $(LIB)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@

indexer.o: $L/webpage.h $C/pagedir.h $C/index.h $C/word.h
indextest.o: $C/index.h $L/file.h

.PHONY: all test clean

test: indexer clean
	bash -v testing.sh

valgrind: indexer
	$(VALGRIND) ./indexer ../data/letters ../data/letters.index

clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f core
