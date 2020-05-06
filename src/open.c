#define _GNU_SOURCE
#include "open.h"
#include "descriptors.h"
#include "log.h"
#include "router.h"
#include "scope.h"
#include <stdio.h>
#include <dlfcn.h>

int open(const char *pathname, int flags) {
	// Check if we are allowed to modify
	if(mayModify()) {
		// Check if we really need to open the file handle, or if reading is overwritten anyway :)
		int routedFd = getReadFdFromName(pathname);
		int readMode = getReadModeFromName(pathname);

		if(routedFd != -1 || readMode == MODE_FAKE || readMode == MODE_DENY) {
			// We won't read from this fd anyway, so why open it?
			return routedFd;
		}
	}


	// Receive descriptor using original function
	int fh = original_open(pathname, flags);

	// Register file descriptor
	registerDescriptor(fh, pathname);

	// Inform user
	logfmt("Opening file %s to file handle %i\n", pathname, fh);

	// Return descriptor
	return fh;
}

// Wrapper for the original open function
int original_open(const char *pathname, int flags) {
	// Link original function if not already linked
	if(!__original_open) {
		__original_open = dlsym(RTLD_NEXT, "open");
	}

	return (*__original_open)(pathname, flags);
}
