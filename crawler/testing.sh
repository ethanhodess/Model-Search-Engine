#!/bin/bash
# Ethan Hodess
# testing.sh - testing file for crawler

# Macros
seedURL1=http://cs50tse.cs.dartmouth.edu/tse/letters/
seedURL2=http://cs50tse.cs.dartmouth.edu/tse/toscrape/
seedURL3=http://cs50tse.cs.dartmouth.edu/tse/wikipedia/

externalURL=https://google.com
invalidURL=http://cs50tse.cs.dartmouth.edu/tse/invalidLink/

pageDirectory1=../data/letters
pageDirectory2=../data/toscrape
pageDirectory3=../data/wikipedia
pageDirectoryInvalid=test

# Remove directories iff exist and make new
#rm -rf ../data/letters
#rm -rf ../data/toscrape
#rm -rf ../data/wikipedia
#mkdir ../data/letters
#mkdir ../data/toscrape
#mkdir ../data/wikipedia

# FAIL 
# 1 Argument
./crawler

# 2 Arguments
./crawler $seedURL1

# 3 arguments
./crawler $seedURL1 $pageDirectory1

# Wrong number of arguments
./crawler $seedURL1 $pageDirectory1 1 dog

# Null argument
./crawler NULL $pageDirectory 1

# URL out of domain
./crawler $externalURL $pageDirectory1 1

# Invalid Directory 
./crawler $seedURL1 $pageDirectoryInvalid 1

# Invalid URL
./crawler $invalidURL $pageDirectory1 1

# Depth too low
./crawler $seedURL1 $pageDirectory1 -1

# Depth too high
./crawler $seedURL1 $pageDirectory1 16


# Should Pass

# Letters tests
./crawler $seedURL1 $pageDirectory1 0

./crawler $seedURL1 $pageDirectory1 1

./crawler $seedURL1 $pageDirectory1 2

./crawler $seedURL1 $pageDirectory1 3

./crawler $seedURL1 $pageDirectory1 4

./crawler $seedURL1 $pageDirectory1 5

./crawler $seedURL1 $pageDirectory1 6

./crawler $seedURL1 $pageDirectory1 7

./crawler $seedURL1 $pageDirectory1 8

./crawler $seedURL1 $pageDirectory1 9

./crawler $seedURL1 $pageDirectory1 10

# Toscrape tests
./crawler $seedURL2 $pageDirectory2 0

./crawler $seedURL2 $pageDirectory2 1

# Wikipedia tests
./crawler $seedURL3 $pageDirectory3 0

./crawler $seedURL3 $pageDirectory3 1

