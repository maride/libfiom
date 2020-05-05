#define _GNU_SOURCE
#include "read.h"
#include "config.h"
#include "descriptors.h"
#include "log.h"
#include "scope.h"
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count) {
	// Link original function if not already linked
	if(!original_read) {
		original_read = dlsym(RTLD_NEXT, "read");
	}

	// Check if we are in scope
	if(mayModify()) {
		// Check if we should simulate a read() call without reading.
		// The behaviour is similar to reading an empty file
		if(!strcmp(getConfigValue("FULL_FAKEREAD"), "TRUE") || (!strcmp(getConfigValue("LOOSE_FAKEREAD"), "TRUE") && fd != 1)) {
			// We're not actually reading.
			// So we just simulate that we read all bytes as requested.
			logfmt("Fakeread 0 bytes from file %s\n", lookupDescriptorName(fd));
			return 0;
		}

		// Check if we should block read() calls
		// This behaviour is similar to reading from a file with unsufficient permissions (or similar)
		if(!strcmp(getConfigValue("FULL_NOREAD"), "TRUE") || (!strcmp(getConfigValue("LOOSE_NOREAD"), "TRUE") && fd != 1)) {
			// We simulate that the file descriptor is not suitable for reading.
			logfmt("Noread from file %s\n", lookupDescriptorName(fd));
			return -1;
		}
	}

	ssize_t read = (*original_read)(fd, buf, count);
	logfmt("Read %li bytes from file %s\n", read, lookupDescriptorName(fd));
	return read;
}
