#define _GNU_SOURCE
#include "read.h"
#include "config.h"
#include "descriptors.h"
#include "log.h"
#include "router.h"
#include "scope.h"
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count) {
	// Check if we are in scope
	if(mayModify()) {
		// Check which mode we should use for reading
		RouteMode mode = getReadMode(fd);

		// Check which fd we should use for reading
		fd = getReadFd(fd);

		// Check if we should simulate a read() call without reading.
		// The behaviour is similar to reading an empty file
		if(mode == MODE_FAKE) {
				// We're not actually reading.
				// So we just simulate that we read all bytes as requested.
				logfmt("Fakeread 0 bytes from file %s\n", lookupDescriptorName(fd));
				return 0;
		}

		// Check if we should block read() calls
		// This behaviour is similar to reading from a file with unsufficient permissions (or similar)
		if(mode == MODE_DENY) {
				// We simulate that the file descriptor is not suitable for reading.
				logfmt("Noread from file %s\n", lookupDescriptorName(fd));
				return -1;
		}
	}

	ssize_t read = original_read(fd, buf, count);
	logfmt("Read %li bytes from file %s\n", read, lookupDescriptorName(fd));
	return read;
}

// Wrapper for the original read function
ssize_t original_read(int fd, void *buf, size_t count) {
	// Link original function if not already linked
	if(!__original_read) {
		__original_read = dlsym(RTLD_NEXT, "read");
	}

	return (*__original_read)(fd, buf, count);	
}
