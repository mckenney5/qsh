/* This library is for Linux system calls */

/* Universal */
#include <strings.h>

/* Linux Specific */
#include <unistd.h>	//checks if file exists
#include <pwd.h>	//gets user name
#include <sys/types.h>	//run()
#include <sys/wait.h>	//run()
#include <sys/stat.h>	//isFile();

/* Global READ ONLY vars */
const char DEFAULT_EXEC_PATH1[] = "/bin/";
const char DEFAULT_EXEC_PATH2[] = "/usr/bin/";
const char DEFAULT_EXEC_PATH3[] = "/sbin";

int get_hostname(char *hostname, size_t len){
	return gethostname(hostname, len);
}

char *get_cwd(char *buf, size_t size){
	return getcwd(buf, size);
}

void get_user(char *buf, size_t size){
	struct passwd *pass;
	pass = getpwuid(getuid());
	strncpy(buf, pass->pw_name, size);
}


