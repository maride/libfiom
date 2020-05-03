#define _GNU_SOURCE
#include "close.h"
#include <stdio.h>
#include <dlfcn.h>

int close(int fd) {
	printf("Closing file handle %i\n", fd);

	int (*original_fclose)(int);
	original_fclose = dlsym(RTLD_NEXT, "close");
	return (*original_fclose)(fd);
}

