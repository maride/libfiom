#include "descriptors.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

DescriptorEntry* descriptors;
int descriptorEntries = 0;

// As this is a shared library the main application is unaware of, we need to take care of "on exit cleanup" on our own.
bool didRegisterDescriptorsCleanup = false;
void registerDescriptorsCleanupFunction() {
	if(!didRegisterDescriptorsCleanup) {
		atexit(doDescriptorCleanup);
		didRegisterDescriptorsCleanup = true;
	}
}

// Does the cleanup on exit
void doDescriptorCleanup() {
	if(descriptors != 0) {
		logfmt("Cleaning up descriptor table...\n");
		free(descriptors);
	}
}

// Reigsters a descriptor and its filename, e.g. after an open().
void registerDescriptor(int fd, const char* filename) {
	// Make sure we're cleaning up on exit
	registerDescriptorsCleanupFunction();

	// Alloc more space
	DescriptorEntry* newEntries = (DescriptorEntry*)malloc((descriptorEntries + 1) * sizeof(DescriptorEntry));
	memcpy(newEntries, descriptors, descriptorEntries * sizeof(DescriptorEntry));

	// Create new entry
	DescriptorEntry newEntry = {
		fd,
		filename,
		true
	};

	// Append entries
	memcpy(&newEntries[descriptorEntries], &newEntry, sizeof(DescriptorEntry));

	// Free old entries
	free(descriptors);

	descriptors = newEntries;
	descriptorEntries++;
}

// Marks a descriptor as closed, e.g. after a close().
void unregisterDescriptor(int fd) {
	for(int i = 0; i < descriptorEntries; i++) {
		if(descriptors[i].fd == fd) {
			descriptors[i].isOpen = false;
			return;
		}
	}

	logfmt("Tried to unregister %i but it is not registered yet.\n", fd);
}

// Returns the DescriptorEntry for the given descriptor, or null if not found.
DescriptorEntry* lookupDescriptor(int fd) {
	for(int i = 0; i < descriptorEntries; i++) {
		if(descriptors[i].fd == fd) {
			return &descriptors[i];
		}
	}

	return 0;
}

// Returns the pathname for the given descriptor, or "<unknown>" if not found.
const char* lookupDescriptorName(int fd) {
	// Special fd's
	if(fd == 0) {
		return "<stdin>";
	} else if(fd == 1) {
		return "<stdout>";
	} else if(fd == 2) {
		return "<stderr>";
	}

	// No special fd's, look into our records
	for(int i = 0; i < descriptorEntries; i++) {
		if(descriptors[i].fd == fd && descriptors[i].isOpen) {
			return descriptors[i].filename;
		}
	}

	// Still not found... throw an error.
	logfmt("Tried to look up unknown descriptor %i\n", fd);
	return "<unknown>";
}

// Returns the first (!) open fd for the given pathname 
int lookupDescriptorFromName(const char* pathname) {
	// Special fd's
	if(!strcmp(pathname, "stdin")) {
		return 0;
	} else if(!strcmp(pathname, "stdout")) {
		return 2;
	} else if(!strcmp(pathname, "stderr")) {
		return 3;
	}

	// No special fd's, look into our records
	for(int i = 0; i < descriptorEntries; i++) {
		if(!strcmp(descriptors[i].filename, pathname) && descriptors[i].isOpen) {
			return descriptors[i].fd;
		}
	}

	// Still not found... throw an error.
	logfmt("Tried to look up descriptor for pathname '%s', but not found in descriptor table.\n", pathname);
	return -1;
}
