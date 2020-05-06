#ifndef __read_h__
#define __read_h__

#include <sys/types.h>

ssize_t read(int fd, void *buf, size_t count);
ssize_t (*__original_read)(int fd, void *buf, size_t count);
ssize_t original_read(int fd, void *buf, size_t count);

#endif // __read_h__
