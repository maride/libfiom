#ifndef __open_h__
#define __open_h__

#include <stdio.h>

int open(const char *pathname, int flags);
int (*__original_open)(const char*, int);
int original_open(const char *pathname, int flags);

#endif // __open_h__
