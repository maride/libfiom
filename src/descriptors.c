#include "descriptors.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

DescriptorEntry* entries;
int numEntries = 0;

// As this is a shared library the main application is unaware of, we need to take care of "on exit cleanup" on our own.
bool didRegisterCleanup = false;
void registerCleanupFunction() {
	if(!didRegisterCleanup) {
		atexit(doCleanup);
		didRegisterCleanup = true;
	}
}

// Does the cleanup on exit
void doCleanup() {
	if(entries != 0) {
		logfmt("Cleaning up descriptor table...\n");
		free(entries);
	}
}

// Reigsters a descriptor and its filename, e.g. after an open().
void registerDescriptor(int fd, const char* filename) {
	// Make sure we're cleaning up on exit
	registerCleanupFunction();

	// Alloc more space
	DescriptorEntry* newEntries = (DescriptorEntry*)malloc((numEntries + 1) * sizeof(DescriptorEntry));
	memcpy(newEntries, entries, numEntries * sizeof(DescriptorEntry));

	// Create new entry
	DescriptorEntry newEntry = {
		fd,
		filename,
		true
	};

	// Append entries
	memcpy(&newEntries[numEntries], &newEntry, sizeof(DescriptorEntry));

	// Free old entries
	free(entries);

	entries = newEntries;
	numEntries++;
}

// Marks a descriptor as closed, e.g. after a close().
void unregisterDescriptor(int fd) {
	for(int i = 0; i < numEntries; i++) {
		if(entries[i].fd == fd) {
			entries[i].isOpen = false;
			return;
		}
	}

	logfmt("Tried to unregister %i but it is not registered yet.\n", fd);
}

// Returns the DescriptorEntry for the given descriptor, or null if not found.
DescriptorEntry* lookupDescriptor(int fd) {
	for(int i = 0; i < numEntries; i++) {
		if(entries[i].fd == fd) {
			return &entries[i];
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
	for(int i = 0; i < numEntries; i++) {
		if(entries[i].fd == fd && entries[i].isOpen) {
			return entries[i].filename;
		}
	}

	// Still not found... throw an error.
	logfmt("Tried to look up unknown descriptor %i\n", fd);
	return "<unknown>";
}
