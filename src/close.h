#ifndef __close_h__
#define __close_h__

int (*original_close)(int);
int close(int fd);

#endif // __close_h__
