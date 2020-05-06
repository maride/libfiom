#ifndef __routedef_h__
#define __routedef_h__

#include "router.h"

void addRoutesFromFile(const char* filename);

void parseLine(char* line);
void skipIgnoredChars(char** line);
RouteMode readMode(char** line);

#endif // __routedef_h__