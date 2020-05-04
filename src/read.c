#define _GNU_SOURCE
#include "read.h"
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count) {
	ssize_t (*original_fread)(int fd, void *buf, size_t count);
	original_fread = dlsym(RTLD_NEXT, "read");
	ssize_t read = (*original_fread)(fd, buf, count);
	printf("Read %li bytes from file handle %i\n", read, fd);
	return read;
}

