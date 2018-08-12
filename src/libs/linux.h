/* This library is for Linux system calls */

/* Universal */
#include <stdio.h>
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

static const char HOME[] = "/home/";

int get_hostname(char *hostname, size_t len){
	return gethostname(hostname, len);
}

char *get_cwd(char *buf, size_t size){
	return getcwd(buf, size);
}

void get_cwd_last(char* buf, size_t size){
	get_cwd(buf, size);
	char *last;
	last = strrchr(buf, '/');
	if(last == NULL)
		perror("Error in program, NULL pointer in get_cwd_last");
	else if(!strcmp("/", last))
		strncpy(buf, last, size);
	else {	
		//copy all but the '/'
		strncpy(buf, last, size);
		unsigned int i;
		for(i=0; buf[i] != '\0'; i++)
			buf[i] = buf[i+1];
	}
}

void get_user(char *buf, size_t size){
	struct passwd *pass;
	pass = getpwuid(getuid());
	strncpy(buf, pass->pw_name, size);
}

void get_home(char *buf, size_t size){
	char user[255];
	get_user(user, sizeof(user));
	snprintf(buf, size, "%s%s", HOME, user); 
}

