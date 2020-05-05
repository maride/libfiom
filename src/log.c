#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "config.h"


void logfmt(char *fmt, ...) {
	if(strcmp(getConfigValue("LOGSTDOUT"), "TRUE")) {
		return;
	}

	printf("[\033[31mlibfiom\033[0m] ");
	va_list argp;
	va_start(argp, fmt); 
	vfprintf(stdout, fmt, argp);
	va_end(argp);
}