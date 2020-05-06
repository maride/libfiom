#ifndef __router_h__
#define __router_h__

#include <stdbool.h>

#define MAXPATHLEN 256

typedef enum RouteMode {
	MODE_TRUE,
	MODE_FAKE,
	MODE_DENY
} RouteMode;

typedef struct {
	const char pathname[MAXPATHLEN];
	int readFd;
	RouteMode readMode;
	int writeFd;
	RouteMode writeMode;
} Route;

void registerRouterCleanupFunction();
void doRouterCleanup();

void addRouterEntry(const char* target, int readFd, RouteMode readMode, int writeFd, RouteMode writeMode);

int getReadFd(int fd);
int getReadFdFromName(const char* pathname);
RouteMode getReadMode(int fd);
RouteMode getReadModeFromName(const char* pathname);
int getWriteFd(int fd);
RouteMode getWriteMode(int fd);

void loadRoutefileOnce();

#endif // __router_h__