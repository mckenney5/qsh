/* This library is for Windows system calls */
/* TODO finish this */
#error Not supported yet

/* Universal */
#include <strings.h>

/* Windows Specific */
#include <windows.h>

/* Custom */
#include "safestring.h"

/* Global READ ONLY vars */
const char DEFAULT_EXEC_PATH1[] = "C:\\Windows\\System32\\";
const char DEFAULT_EXEC_PATH2[] = "";
const char DEFAULT_EXEC_PATH3[] = "";

int get_hostname(char *hostname, size_t len){
	//return gethostname(hostname, len);
}

char *get_cwd(char *buf, size_t size){
	//return getcwd(buf, size);
}

void get_user(char *buf, size_t size){
	//struct passwd *pass;
	//pass = getpwuid(getuid());
	//strncpy2(buf, pass->pw_name, size);
}


