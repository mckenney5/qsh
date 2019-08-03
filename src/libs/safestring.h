/* This library is for custom string functions */
#ifndef __SAFESTRING_H
#define __SAFESTRING_H

char *strncpy2(char *dest, const char *src, size_t n){
/* Used to fix bug where strings might not end in a null */
	size_t i;
	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[i] = src[i];
	for ( ; i < n; i++)
		dest[i] = '\0';
	dest[sizeof(dest)/sizeof(dest[0])] = '\0'; //Ignore this warning
	/* note that gcc will make this a warning */
	/* if the string does not end in null then strlen wont work */
	/* this has worked in testing for use on this project */
	return dest;
}
#endif

