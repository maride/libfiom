#define _GNU_SOURCE
#include "read.h"
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count) {
	// Link original function if not already linked
	if(!original_read) {
		original_read = dlsym(RTLD_NEXT, "read");
	}

	ssize_t read = (*original_read)(fd, buf, count);
	printf("Read %li bytes from file handle %i\n", read, fd);
	return read;
}

