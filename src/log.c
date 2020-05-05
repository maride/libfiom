#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "scope.h"


void logfmt(char *fmt, ...) {
	// Check if we should log
	if(strcmp(getConfigValue("LOGSTDOUT"), "TRUE") || !mayModify()) {
		return;
	}

	printf("[\033[31mlibfiom\033[0m] ");
	va_list argp;
	va_start(argp, fmt); 
	vfprintf(stdout, fmt, argp);
	va_end(argp);
}