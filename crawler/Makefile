# Ethan Hodess
# Makefile for crawler 

L = ../libcs50
C = ../common

OBJS = crawler.o
LIB = $C/common.a $L/libcs50.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: crawler

crawler: $(OBJS) $(LIB)
	$(CC) $(CFLAGS) $^ $(LIB) -o $@

crawler.o: $L/bag.h $L/hashtable.h $L/webpage.h $C/pagedir.h 

.PHONY: all test clean

test: crawler clean
	bash -v testing.sh

valgrind: crawler
	$(VALGRIND) ./crawler http://cs50tse.cs.dartmouth.edu/tse/ data/main 1

clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f core
	rm -f data/main/*
	rm -f data/letters/*
	rm -f data/toscrape/*
	rm -f data/wikipedia/*
