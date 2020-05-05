#define _GNU_SOURCE
#include "write.h"
#include "config.h"
#include "descriptors.h"
#include "log.h"
#include "scope.h"
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>


ssize_t write(int fd, const void *buf, size_t count) {
	// Link original function if not already linked
	if(!original_write) {
		original_write = dlsym(RTLD_NEXT, "write");
	}

	// Check if we are in scope
	if(mayModify()) {
		// Check if we should simulate a write() call without writing out
		if(!strcmp(getConfigValue("FULL_FAKEWRITE"), "TRUE") || (!strcmp(getConfigValue("LOOSE_FAKEWRITE"), "TRUE") && fd != 2)) {
			// We're not actually writing out.
			// So we just simulate that we wroute out all bytes as requested.
			logfmt("Fakewrite 0 bytes to file %s\n", lookupDescriptorName(fd));
			return count;
		}

		// Check if we should block write() calls
		if(!strcmp(getConfigValue("FULL_NOWRITE"), "TRUE") || (!strcmp(getConfigValue("LOOSE_NOWRITE"), "TRUE") && fd != 2)) {
			// We simulate that the file descriptor is not suitable for writing.
			logfmt("Nowrite to file %s\n", lookupDescriptorName(fd));
			return -1;
		}
	}

	ssize_t written = (*original_write)(fd, buf, count);
	logfmt("Write %li bytes to file %s\n", written, lookupDescriptorName(fd));
	return written;
}
