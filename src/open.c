#define _GNU_SOURCE
#include "open.h"
#include "descriptors.h"
#include "log.h"
#include <stdio.h>
#include <dlfcn.h>

int open(const char *pathname, int flags) {
	// Link original function if not already linked
	if(!original_open) {
		original_open = dlsym(RTLD_NEXT, "open");
	}

	// Receive descriptor using original function
	int fh = (*original_open)(pathname, flags);

	// Register file descriptor
	registerDescriptor(fh, pathname);

	// Inform user
	logfmt("Opening file %s to file handle %i\n", pathname, fh);

	// Return descriptor
	return fh;
}
