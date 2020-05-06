#define _GNU_SOURCE
#include "write.h"
#include "config.h"
#include "descriptors.h"
#include "log.h"
#include "router.h"
#include "scope.h"
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>


ssize_t write(int fd, const void *buf, size_t count) {
	// Check if we are in scope
	if(mayModify()) {
		// Check which mode we should use for writing
		RouteMode mode = getWriteMode(fd);

		// Check which fd we should use for writing
		fd = getWriteFd(fd);

		// Check if we should simulate a write() call without writing.
		// The behaviour is similar to writing an empty file
		if(mode == MODE_FAKE) {
				// We're not actually writing.
				// So we just simulate that we wrote all bytes as requested.
				logfmt("Fakewrite 0 bytes to file %s\n", lookupDescriptorName(fd));
				return count;
		}

		// Check if we should block write() calls
		// This behaviour is similar to writing from a file with unsufficient permissions (or similar)
		if(mode == MODE_DENY) {
				// We simulate that the file descriptor is not suitable for writing.
				logfmt("Nowrite to file %s\n", lookupDescriptorName(fd));
				return -1;
		}
	}

	ssize_t written = original_write(fd, buf, count);
	logfmt("Write %li bytes to file %s\n", written, lookupDescriptorName(fd));
	return written;
}

// Wrapper for the original write function
ssize_t original_write(int fd, const void *buf, size_t count) {
	// Link original function if not already linked
	if(!__original_write) {
		__original_write = dlsym(RTLD_NEXT, "write");
	}

	return (*__original_write)(fd, buf, count);	
}
