#define _GNU_SOURCE
#include "write.h"
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t count) {
	ssize_t (*original_fwrite)(int fd, const void *buf, size_t count);
	original_fwrite = dlsym(RTLD_NEXT, "write");
	ssize_t written = (*original_fwrite)(fd, buf, count);
	printf("Write %li bytes to file handle %i\n", written, fd);
	return written;
}

