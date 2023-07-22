#!/bin/bash
# Ethan Hodess

# Macros
pageDirectory1=~/cs50-dev/tse-ethanhodess/data/letters
pageDirectory2=~/cs50-dev/tse-ethanhodess/data/toscrape
pageDirectory3=~/cs50-dev/tse-ethanhodess/data/wikipedia

indexFilename1=~/cs50-dev/tse-ethanhodess/data/letters.index
indexFilename2=~/cs50-dev/tse-ethanhodess/data/toscrape.index
indexFilename3=~/cs50-dev/tse-ethanhodess/data/wikipedia.index

invalidFilename=an/invalid/filename
invalidDirectory=~/cs50-dev/tse-ethanhodess/invalid
nonexistentDirectory=~/cs50-dev/tse-ethanhodess/doesNotExist

# FAIL 
# 1 argument
./querier

# 2 arguments
./querier $pageDirectory1

# Too many arguments
./querier $pageDirectory1 $indexFilename1 $indexFilename2

# Invalid filename
./querier $pageDirectory1 $invalidFilename

# Invalid directory
./querier $invalidDirectory $indexFilename1

# Nonexistent directory
./querier $nonexistentDirectory $indexFilename1



# PASS 
# Invalid Input
echo '12' | ./querier $pageDirectory2 $indexFilename2
echo $?

echo 'dog-cat' | ./querier $pageDirectory1 $indexFilename1
echo $?

echo 'animals:' | ./querier $pageDirectory1 $indexFilename1
echo $?

# white space
echo '' | ./querier $pageDirectory1 $indexFilename1
echo $?

echo '          ' | ./querier $pageDirectory1 $indexFilename1
echo $?

echo '   dog         ' | ./querier $pageDirectory1 $indexFilename1
echo $?

# test with bad operator placement
echo 'and home' | ./querier $pageDirectory1 $indexFilename1
echo $?

echo 'home or' | ./querier $pageDirectory1 $indexFilename1
echo $?

echo 'home and or dog' | ./querier $pageDirectory1 $indexFilename1
echo $?

# Basic
echo 'search' | ./querier $pageDirectory1 $indexFilename1
echo $?

# Complex
echo 'graph and coding or biology' | ./querier $pageDirectory1 $indexFilename1
echo $?

# Does not appear
echo 'dog' | ./querier $pageDirectory1 $indexFilename1
echo $?

# AND 
echo 'dog home' | ./querier $pageDirectory1 $indexFilename1
echo $?

echo 'fiction brief' | ./querier $pageDirectory2 $indexFilename2
echo $?

# OR 
echo 'classic or fiction' | ./querier $pageDirectory2 $indexFilename2
echo $?


echo 'home' | make valgrind
