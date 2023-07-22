### Crawler

The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a "seed" URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given "depth."

### Comments and Assumptions
No additional comments or assumptions for **crawler**.

### Files

* `Makefile` - compilation procedure
* `crawler.c` - main code
* `README.md` - info about crawler
* `testing.sh` - testing script
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, `make all` or `make crawler.o`

