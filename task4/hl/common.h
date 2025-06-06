#ifndef HLDETECTOR_COMMON_H
#define HLDETECTOR_COMMON_H

#include <stdio.h>
#include <string.h>

/*
 * Compatibility definition.
 */
size_t	 strlcat(char *, const char *, size_t)
		__attribute__ ((__bounded__(__string__,1,3)));

/*
 * When greater than zero, makes internals print.
 */
extern int hl_debug;

#define dbgprint(...)	do { \
		if (hl_debug) \
			fprintf(stderr, __VA_ARGS__ ); \
	} while (0)

/*
 * Calculates number of non-NULL pointers in arr, before the first NULL one.
 */
size_t arrsz(const char **arr);

#endif
