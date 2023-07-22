/* 
 * Ethan Hodess
 * crawler.c - crawler for tiny search engine
 *
 * The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a "seed" URL.
 * It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but 
 * limiting its exploration to a given "depth".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pagedir.h>
#include <bag.h>
#include <hashtable.h>
#include <webpage.h>

// Function prototypes
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/*
 * The main function simply calls parseArgs and crawl, then exits zero.
 */
int 
main(const int argc, char* argv[])
{
    // converting maxDepth to int
    int maxDepth = -1;
    if (argv[3] != NULL) {
        sscanf(argv[3], "%d", &maxDepth);
    }

    parseArgs(argc, argv, &argv[1], &argv[2], &maxDepth);
    crawl(argv[1], argv[2], maxDepth);
    exit(0);
}

/*
 * Given arguments from the command line, extract them into the function parameters; return only if successful.
 * If any trouble is found, print an error to stderr and exit non-zero.
 */
static void 
parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth)
{
  // check for appropriate number of arguments
  if (argc != 4) {
    fprintf(stderr, "Usage: ./crawler [seedURL] [pageDirectory] [maxDepth]\n");
    exit(1);
  }
    
  // null arguments check
  if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) { 
    fprintf (stderr, "Error: null argument\n");
    exit(1);
  }

  // normalize the URL
  char* normalized = normalizeURL(*seedURL);
  
  // check if valid
  if (normalized == NULL) {
    fprintf(stderr, "Error: invalid URL\n");
    free(normalized);
    exit(1);
  }

  // check that URL is within proper domain
  if (!isInternalURL(normalized)) {
    fprintf(stderr, "Error: not an internal URL\n");
    free(normalized);
    exit(1);
  }

  free(normalized);

  // for pageDirectory, call pagedir_init()
  if (!pagedir_init(*pageDirectory)) {
    // error message is in pagedir_init()
    exit(1);
  }

  // for maxDepth, ensure it is an integer in specified range
  if (*maxDepth < 0 || *maxDepth > 10) {
    fprintf(stderr, "Error: maxDepth out of bounds\n");
    exit(1);
  }
}

/*
 * Performs crawl - while bag is not empty, get a webpage from the bag, fetch HTML, save its data to a file, 
 * and get embedded URLs
 */
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
  int idCounter = 1;
  // initialize hashtable
  hashtable_t* visitedURLs = hashtable_new(200);
  hashtable_insert(visitedURLs, seedURL, "");

  // initialize bag
  bag_t* toCrawl = bag_new();

  // create a copy of the seedURL to use for the webpage
  char* seedURLNew = malloc(strlen(seedURL) + 1);

  // memory check
  if (seedURLNew == NULL) {
    fprintf(stderr, "Error: memory\n");
    exit(1);
  }

  strcpy(seedURLNew, seedURL);
  
  webpage_t* seedURLWebpage = webpage_new(seedURLNew, 0, NULL);
  bag_insert(toCrawl, seedURLWebpage);

  webpage_t* currentPage;

  // while bag is not empty, pull a webpage from the bag
  while ((currentPage = bag_extract(toCrawl)) != NULL) {
    // fetch the HTML for that webpage
    if (webpage_fetch(currentPage)) {
      //if fetch was successful
      fprintf(stdout, "%d \t Fetched \t %s\n", webpage_getDepth(currentPage), webpage_getURL(currentPage));

      // save the webpage to pageDirectory
      pagedir_save(currentPage, pageDirectory, idCounter);

      // if the webpage is not at maxDepth
      if (webpage_getDepth(currentPage) < maxDepth) {
        fprintf(stdout, "%d \t Scanning \t %s\n", webpage_getDepth(currentPage), webpage_getURL(currentPage));

        // pageScan that HTML
        pageScan(currentPage, toCrawl, visitedURLs);
      }
    }
    // delete that webpage
    webpage_delete(currentPage);

    idCounter += 1;

  }
  hashtable_delete(visitedURLs, NULL);
  bag_delete(toCrawl, NULL);
}

/*
 * Scan a given page and insert the links into the hashtable and bag
 */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
  int pos = 0;
  char* URL;
     
  // while there is another URL in the page
  while ((URL = webpage_getNextURL(page, &pos)) != NULL) {

    char* newURL = normalizeURL(URL);
    fprintf(stdout, "%d \t Found \t \t %s\n", webpage_getDepth(page), newURL);

    // if that URL is Internal
    if (isInternalURL(newURL)) {
      // insert the webpage into the hashtable
      if (hashtable_insert(pagesSeen, newURL, "")) {
        // if that succeded
        fprintf(stdout, "%d \t Added \t \t %s\n", webpage_getDepth(page), newURL);

        // create a webpage_t for it
        webpage_t* newPage = webpage_new(newURL, webpage_getDepth(page) + 1, NULL);

        // insert the webpage into the bag
        bag_insert(pagesToCrawl, newPage);
      } else {
        fprintf(stdout, "%d \t IgnDupl \t %s\n", webpage_getDepth(page), newURL);
        free(newURL);
      }
    } else {
      fprintf(stdout, "%d \t IgnExtrn \t %s\n", webpage_getDepth(page), newURL);
      free(newURL);
    }
    free(URL);
  }
}

