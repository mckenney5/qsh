/* This library is for Linux system calls */

#ifndef _using_linux_h
#define _using_linux_h

/* Universal */
#include <iostream>

/* Linux Specific */
#include <unistd.h>	//checks if file exists
#include <pwd.h>	//gets user name
#include <sys/types.h>	//run()
#include <sys/wait.h>	//run()
#include <sys/stat.h>	//isFile();


#define _MAX_DIR_NAME 210

using namespace std;

/* Global READ ONLY vars */
const char DEFAULT_EXEC_PATH1[] = "/usr/bin/";
const char DEFAULT_EXEC_PATH2[] = "/bin/";
const char DEFAULT_EXEC_PATH3[] = "/sbin";

static const char HOME[] = "/home/";

int get_hostname(char *hostname, size_t len){
/* Used to get the computers name */
	return gethostname(hostname, len);
}

char *get_cwd(char *buf, size_t size){
/* Used to get the current working dir */
	return getcwd(buf, size);
}

void get_cwd_last(char* buf, size_t size){
/* Used to get the name of the dir we are in */
	get_cwd(buf, size);
	char *last;
	last = strrchr(buf, '/');
	char temp[_MAX_DIR_NAME] = {'\0'};
	strncpy2(temp, last, _MAX_DIR_NAME);
	if(last == NULL)
		perror("qsh : Error in program, NULL pointer in get_cwd_last");
	else if(!strcmp("/", temp))
		strncpy2(buf, temp, size);
	else {	
		//copy all but the '/'
		strncpy2(buf, temp, size);
		unsigned int i;
		for(i=0; buf[i] != '\0'; i++)
			buf[i] = buf[i+1];
	}
}

void get_user(char *buf, size_t size){
/* Used to get username */
	struct passwd *pass;
	pass = getpwuid(getuid());
	strncpy2(buf, pass->pw_name, size);
}

void get_home(char *buf, size_t size){
/* Used to get the user's home dir location */
	char user[255] = {'\0'};
	get_user(user, 255);
	snprintf(buf, size, "%s%s", HOME, user); //ignore this warning, truncation is fine 
}

#endif

