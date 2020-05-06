#ifndef __descriptors_h__
#define __descriptors_h__

#include <stdbool.h>

typedef struct {
	int fd;
	const char* filename;
	bool isOpen;
} DescriptorEntry;

void registerDescriptorsCleanupFunction();
void doDescriptorCleanup();

void registerDescriptor(int fd, const char* filename);
void unregisterDescriptor(int fd);
DescriptorEntry* lookupDescriptor(int fd);
const char* lookupDescriptorName(int fd);
int lookupDescriptorFromName(const char* pathname);

#endif // __descriptors_h__