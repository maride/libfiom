#define _GNU_SOURCE
#include "scope.h"
#include "config.h"
#include <errno.h>
#include <string.h>

// Checks if there is a scope, and if there is a scope, if the current executable is in scope. Returns true if the calls may be modified, or if they should remain untouched.
bool mayModify() {
	// Check if we even need to check the scope
	if(!strcmp(getConfigValue("LIMIT_SCOPE_TO"), "")) {
		return true;
	}

	// Check if executable name matches desired executable
	if(strcmp(program_invocation_name, getConfigValue("LIMIT_SCOPE_TO"))) {
		// Wrong executable. Do not modify anything.
		return false;
	}

	return true;
}
