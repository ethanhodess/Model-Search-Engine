# Ethan Hodess
# Makefile for 'common' library

L = ../libcs50

OBJS = pagedir.o index.o word.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# Library Build - Archive Object Files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h $L/webpage.h $L/file.h
index.o: index.h $L/hashtable.h $L/counters.h $L/file.h
word.o: word.h

.PHONY: clean

clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f $(LIB) *~ *.o
	rm -f core

