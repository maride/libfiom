#include "log.h"
#include <stdarg.h>
#include <stdio.h>


void logfmt(char *fmt, ...) {
	printf("[\033[31mlibfiom\033[0m] ");
	va_list argp;
	va_start(argp, fmt); 
	vfprintf(stdout, fmt, argp);
	va_end(argp);
}