#ifndef __write_h__
#define __write_h__

#include <sys/types.h>

ssize_t (*original_write)(int fd, const void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);

#endif // __write_h__
