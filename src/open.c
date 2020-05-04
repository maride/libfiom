#define _GNU_SOURCE
#include "open.h"
#include "log.h"
#include <stdio.h>
#include <dlfcn.h>

int open(const char *pathname, int flags) {
	// Link original function if not already linked
	if(!original_open) {
		original_open = dlsym(RTLD_NEXT, "open");
	}

	int fh = (*original_open)(pathname, flags);
	logfmt("Opening file %s to file handle %i\n", pathname, fh);
	return fh;
}

