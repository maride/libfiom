#define _GNU_SOURCE
#include "write.h"
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t count) {
	// Link original function if not already linked
	if(!original_write) {
		original_write = dlsym(RTLD_NEXT, "write");
	}

	ssize_t written = (*original_write)(fd, buf, count);
	printf("Write %li bytes to file handle %i\n", written, fd);
	return written;
}

