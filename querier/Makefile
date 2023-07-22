# Ethan Hodess
# Makefile for querier

L = ../libcs50
C = ../common

OBJS = querier.o
LIB = $C/common.a $L/libcs50.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# By default, the Makefile builds 'querier'.
all: querier

querier: querier.o $(LIB)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@

querier.o: $L/file.h $L/counters.h $C/index.h $C/pagedir.h $C/word.h

.PHONY: all test clean

test: querier clean
	bash -v testing.sh

valgrind: querier
	$(VALGRIND) ./querier ~/cs50-dev/shared/tse/output/data/toscrape-depth-1 ~/cs50-dev/shared/tse/output/data/toscrape-index-1

clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f core
