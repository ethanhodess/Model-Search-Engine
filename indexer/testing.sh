#!/bin/bash
# Ethan Hodess
# testing.sh - testing file for indexer

pageDirectory1=~/cs50-dev/tse-ethanhodess/data/letters
pageDirectory2=~/cs50-dev/tse-ethanhodess/data/toscrape
pageDirectory3=~/cs50-dev/tse-ethanhodess/data/wikipedia

invalidDirectory=~/cs50-dev/tse-ethanhodess/invalid
nonexistentDirectory=~/cs50-dev/tse-ethanhodess/non-existent
invalidFilename=invalid/filename

indexFilename1=~/cs50-dev/tse-ethanhodess/data/letters.index
indexFilename2=~/cs50-dev/tse-ethanhodess/data/toscrape.index
indexFilename3=~/cs50-dev/tse-ethanhodess/data/wikipedia.index

# FAIL 
# 1 Argument
./indexer

# 2 Arguments
./indexer $pageDirectory1

# Wrong number of arguments
./indexer $pageDirectory1 $indexFilename1 dog

# Invalid Filename
./indexer $pageDirectory1 $invalidFilename 

# Non-Existent Directory
./indexer $nonexistentDirectory $indexFilename1

# Invalid Directory
./indexer $invalidDirectory $indexFilename1



# PASS 

# Letters
./indexer $pageDirectory1 $indexFilename1
echo $?

# Toscrape
./indexer $pageDirectory2 $indexFilename2
echo $?

# wikipedia
./indexer $pageDirectory3 $indexFilename3
echo $?

# Valgrind
make valgrind



# Indextest

# Letters
./indextest $indexFilename1 ~/cs50-dev/tse-ethanhodess/data/new_letters.index
echo $?

# Toscrape
./indextest $indexFilename2 ~/cs50-dev/tse-ethanhodess/data/new_toscrape.index
echo $?

# Wikipedia
./indextest $indexFilename3 ~/cs50-dev/tse-ethanhodess/data/new_wikipedia.index
echo $?


# Comparison using indexcmp
~/cs50-dev/shared/tse/indexcmp $indexFilename1 ~/cs50-dev/tse-ethanhodess/data/new_letters.index

~/cs50-dev/shared/tse/indexcmp $indexFilename2 ~/cs50-dev/tse-ethanhodess/data/new_toscrape.index

~/cs50-dev/shared/tse/indexcmp $indexFilename3 ~/cs50-dev/tse-ethanhodess/data/new_wikipedia.index
