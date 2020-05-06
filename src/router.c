#include "router.h"
#include "close.h"
#include "config.h"
#include "descriptors.h"
#include "log.h"
#include "open.h"
#include "read.h"
#include "routedef.h"
#include <stdlib.h>
#include <string.h>

Route *routes;
int numRoutes = 0;
bool routefileLoaded;

// As this is a shared library the main application is unaware of, we need to take care of "on exit cleanup" on our own.
bool didRouterCleanup = false;
void registerRouterCleanupFunction() {
	if(!didRouterCleanup) {
		atexit(doRouterCleanup);
		didRouterCleanup = true;
	}
}

// Does the cleanup on exit
void doRouterCleanup() {
	if(routes != 0) {
		logfmt("Cleaning up routing table...\n");
		free(routes);
	}
}

// Adds an entry to the router table
void addRouterEntry(const char* target, int readFd, RouteMode readMode, int writeFd, RouteMode writeMode) {
	// Make sure we're cleaning up on exit
	registerRouterCleanupFunction();

	// Alloc more space
	Route* newRoutes = (Route*)malloc((numRoutes + 1) * sizeof(Route));
	memcpy(newRoutes, routes, numRoutes * sizeof(Route));

	// Create new entry
	Route newEntry;
	strncpy((char*)newEntry.pathname, target, MAXPATHLEN);
	newEntry.readFd = readFd;
	newEntry.readMode = readMode;
	newEntry.writeFd = writeFd;
	newEntry.writeMode = writeMode;

	// Append entries
	memcpy(&newRoutes[numRoutes], &newEntry, sizeof(Route));

	// Free old entries
	free(routes);

	routes = newRoutes;
	numRoutes++;	
}

// Returns the readFd for the given fd
int getReadFd(int fd) {
	int targetFd = getReadFdFromName(lookupDescriptorName(fd));
	if(targetFd == -1) {
		// Not found, default to the original fd then
		return fd;
	}

	return targetFd;
}

// Returns the readFd for the given pathname
int getReadFdFromName(const char* pathname) {
	// Search in routing table if there is a matching entry
	// Load route file
	loadRoutefileOnce();

	// Search through routing table
	for(int i=0; i<numRoutes; i++) {
		if(!strcmp(routes[i].pathname, pathname)) {
			return routes[i].readFd;
		}
	}

	// There is no matching entry, we need to use the given fd
	return -1;
}

// Returns the mode to read the specified fd with
RouteMode getReadMode(int fd) {
	return getReadModeFromName(lookupDescriptorName(fd));
}

// Returns the mode to read the specified pathname with
RouteMode getReadModeFromName(const char* pathname) {
	// Search in routing table if there is a matching entry
	// Load route file
	loadRoutefileOnce();

	// Search through routing table
	for(int i=0; i<numRoutes; i++) {
		if(!strcmp(routes[i].pathname, pathname)) {
			return routes[i].readMode;
		}
	}

	// Not found... We should stay with the normal function of read()
	return MODE_TRUE;
}

// Returns the writeFd for the given fd
int getWriteFd(int fd) {
	// Search in routing table if there is a matching entry
	const char* pathname = lookupDescriptorName(fd);

	// Load route file
	loadRoutefileOnce();

	// Search through routing table
	for(int i=0; i<numRoutes; i++) {
		if(!strcmp(routes[i].pathname, pathname)) {
			return routes[i].writeFd;
		}
	}

	// There is no matching entry, we need to use the given fd
	return fd;
}

// Returns the mode to write the specified fd with
RouteMode getWriteMode(int fd) {
	// Search in routing table if there is a matching entry
	const char* pathname = lookupDescriptorName(fd);

	// Load route file
	loadRoutefileOnce();

	// Search through routing table
	for(int i=0; i<numRoutes; i++) {
		if(!strcmp(routes[i].pathname, pathname)) {
			return routes[i].writeMode;
		}
	}

	// Not found... We should stay with the normal function of write()
	return MODE_TRUE;
}

// Loads the routefile, as specified in LIBFIOM_ROUTEFILE, once.
void loadRoutefileOnce() {
	if(!routefileLoaded) {
		// Read filename out of config
		const char* filename = getConfigValue("ROUTEFILE");

		// check if filename is set
		if(strcmp(filename, "")) {
			addRoutesFromFile(filename);
		}

		// make sure that route file is not read again
		routefileLoaded = true;
	}
}
