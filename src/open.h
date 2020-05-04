#ifndef __open_h__
#define __open_h__

#include <stdio.h>

int (*original_open)(const char*, int);
int open(const char *pathname, int flags);

#endif // __open_h__
