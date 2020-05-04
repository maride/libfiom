#define _GNU_SOURCE
#include "open.h"
#include <stdio.h>
#include <dlfcn.h>

int open(const char *pathname, int flags) {
	int (*original_fopen)(const char*, int);
	original_fopen = dlsym(RTLD_NEXT, "open");
	int fh = (*original_fopen)(pathname, flags);
	printf("Opening file %s to file handle %i\n", pathname, fh);
	return fh;
}

