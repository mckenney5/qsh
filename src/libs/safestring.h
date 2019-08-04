/* This library is for custom string functions */
#ifndef __SAFESTRING_H
#define __SAFESTRING_H

#include <string.h>

char *strncpy2(char *dest, const char *src, size_t n){
//return strncpy(dest, src, n);
/* Used to fix bug where strings might not end in a null */
	
	#ifdef DEBUGGING
		printf("[DEBUG] Safestring: dest '%s', src '%s', size '%ld'\n", dest, src, n);
	#endif

	size_t i;
	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[i] = src[i];
	for ( ; i < n; i++)
		dest[i] = '\0';
	dest[n-1] = '\0'; //HACK
	/* Though the hack will make sure the string ends in null, the string */
	/* might have garbage inbetween or worst case a segfault if given n > dest size */
	/* TODO find a better way, note strlen() is not applicable here */
	return dest;
}
#endif

