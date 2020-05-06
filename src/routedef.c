#include "routedef.h"
#include "close.h"
#include "descriptors.h"
#include "log.h"
#include "open.h"
#include "read.h"
#include <stdio.h>
#include <string.h>

#define ROUTEFILE_MAXSIZE 65536

// Adds all routes as specified in the file specified
void addRoutesFromFile(const char* filename) {
	// Read file
	char buf[ROUTEFILE_MAXSIZE];
	int fh = original_open(filename, 0);
	original_read(fh, &buf, ROUTEFILE_MAXSIZE);
	original_close(fh);

	// Temporary vars
	char tmpFilename[MAXPATHLEN];
	int tmpReadFd = -1;
	RouteMode tmpReadMode;
	int tmpWriteFd = -1;
	RouteMode tmpWriteMode;

	// Parse file
	char* tmpPart = strtok(buf, ";\n");
	while(tmpPart != NULL) {
		skipIgnoredChars(&tmpPart);

		// Check what kind of line we have
		if(!strncmp(tmpPart, "file", 4)) {
			// Check if we already read another route entry - we need to save it before
			addRouterEntry(tmpFilename, tmpReadFd, tmpReadMode, tmpWriteFd, tmpWriteMode);

			// Focus on this file
			tmpPart += 4;
			skipIgnoredChars(&tmpPart);
			strncpy(tmpFilename, tmpPart, MAXPATHLEN);
		} else if(!strncmp(tmpPart, "read", 4)) {
			// Get read target
			tmpPart += 4;
			skipIgnoredChars(&tmpPart);


			// Get mode
			tmpReadMode = readMode(&tmpPart);
			skipIgnoredChars(&tmpPart);

			// Check if the line is already at the end
			if(tmpPart != 0) {
				// try to open
				int fd = original_open(tmpPart, 0);

				if(fd < 0) {
					logfmt("Failed to open \"%s\" for reading.\n", tmpPart);
				} else {
					tmpReadFd = fd;

					// Also register that file handle
					registerDescriptor(fd, tmpPart);
				}
			}
		} else if(!strncmp(tmpPart, "write", 5)) {
			// Get write target
			tmpPart += 5;
			skipIgnoredChars(&tmpPart);


			// Get mode
			tmpWriteMode = readMode(&tmpPart);
			skipIgnoredChars(&tmpPart);

			// Check if the line is already at the end
			if(tmpPart != 0) {
				// try to open
				int fd = original_open(tmpPart, 0);

				if(fd < 0) {
					logfmt("Failed to open \"%s\" for reading.\n", tmpPart);
				} else {
					tmpWriteFd = fd;

					// Also register that file handle
					registerDescriptor(fd, tmpPart);
				}
			}
		} else {
			logfmt("Ignoring line \"%s\"\n", tmpPart);
		}

		// Go to next part
		tmpPart = strtok(NULL, ";\n");
	}

	// Add (last) router entry
	addRouterEntry(tmpFilename, tmpReadFd, tmpReadMode, tmpWriteFd, tmpWriteMode);	
}

// Move char pointer as long as it points to an ignored character
void skipIgnoredChars(char** line) {
	while(*line[0] != 0 && (*line[0] == ' ' || *line[0] == '\t')) {
		(*line)++;
	}
}

// requires line to point to the mode as a string
RouteMode readMode(char** line) {
	if(!strncmp(*line, "true", 4)) {
		*line += 4;
		return MODE_TRUE;
	} else if(!strncmp(*line, "fake", 4)) {
		*line += 4;
		return MODE_FAKE;
	} else if(!strncmp(*line, "deny", 4)) {
		*line += 4;
		return MODE_DENY;
	}

	logfmt("Unknown mode encountered (\"%s\")\n", line);
	return MODE_DENY;
}
