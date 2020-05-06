#ifndef __close_h__
#define __close_h__

int close(int fd);
int (*__original_close)(int);
int original_close(int fd);

#endif // __close_h__
