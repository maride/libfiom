#include "config.h"
#include <stdio.h>
#include <stdlib.h>

char emptyValue[0];

const char* getConfigValue(char* key) {
	// Just return the corresponding env var. TODO: read from a config file instead
	char prefixedKey[64];
	snprintf(prefixedKey, 64, "LIBFIOM_%s", key);
	char* value = getenv(prefixedKey);
	
	// Check if the env is really set - if not, return an empty string to avoid null pointer problems
	if(!value) {
		return emptyValue;
	}
	return value;
}
