#define _GNU_SOURCE
#include "close.h"
#include "log.h"
#include <stdio.h>
#include <dlfcn.h>

int close(int fd) {
	// Link original function if not already linked
	if(!original_close) {
		original_close = dlsym(RTLD_NEXT, "close");
	}

	logfmt("Closing file handle %i\n", fd);
	return (*original_close)(fd);
}

