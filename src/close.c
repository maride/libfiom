#define _GNU_SOURCE
#include "close.h"
#include "descriptors.h"
#include "log.h"
#include <stdio.h>
#include <dlfcn.h>

int close(int fd) {

	logfmt("Closing file %s (handle %i)\n", lookupDescriptorName(fd), fd);

	// Unregister descriptor
	unregisterDescriptor(fd);

	return original_close(fd);
}

// Wrapper for the original close function
int original_close(int fd) {
	// Link original function if not already linked
	if(!__original_close) {
		__original_close = dlsym(RTLD_NEXT, "close");
	}

	return (*__original_close)(fd);
}
