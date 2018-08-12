/* Quick Shell's main header file */
/*
 * Compiler Options:
 * -DGNU 	: The user interface uses GNU Readline
 * -DTINY 	: The user interface just uses fgets() (aka doesnt handle line editing)
 * <none>	: The user interface uses linenoise library 
 *		  (see https://github.com/antirez/linenoise)
 * -DNO_COLOR	: Disables color for the prompt (note does not affect readline nor linenoise)
 */

/* Standard C Libs */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(unix) || defined(__unix__) || defined(__unix)
/* Standard Linux Libs */
	#include "libs/linux.h"
#else
	#warn Operating system is not UNIX based, system calls may not work
	//eventually check if _WIN32, then include libs/windows.h
	#include "libs/linux.h"
#endif

/* Non-standard libs */
#ifdef GNU
	#include <readline/readline.h>
	#include <readline/history.h>
#else
	#ifndef TINY
		//if neither GNU nor TINY is defined, use this as the default
		#include "libs/linenoise/linenoise.h"
		#define HIST_FILE "history.log"
	#endif
#endif

/* Commands */
#include "commands/c.h"
#include "commands/help.h"

#define MAX_USER_INPUT 1024 //how much data can be typed in the terminal
#define BAD_COMMAND "qsh : Command not found." //What is returned when the command isnt found
#define MAX_ARGS 4096 //POSIX smallest ammount of command line args

#ifdef DEBUGGING
	#define DEBUG 1
#else
	#define DEBUG 0
#endif

/* Foreground Colors */
#ifndef NO_COLOR
	#define RED "\x1B[31m"
	#define GREEN "\x1B[32m"
	#define YELLOW "\x1B[33m"
	#define BLUE "\x1B[34m"
	#define MAGENTA "\x1B[35m"
	#define CYAN "\x1B[36m"
	#define WHITE "\x1B[37m"
	#define RESET "\x1B[0m"
#else
	#define RED ""
	#define GREEN ""
	#define YELLOW ""
	#define BLUE ""
	#define MAGENTA ""
	#define CYAN ""
	#define WHITE ""
	#define RESET ""
#endif

#define DEFAULT_PROMPT "=> "

static void interp(char[]); //checks if commands are valid, then runs them
static int find(const char[], const int, char*[]); //checks if a file exists, puts its location in *location
static int handle_var(char**);
static void run(const char*, char*, char*);
static int call(const char *location, char *program, char *argv[], int argc);
static int isFile(const char* path); //checks if the path leads to a file
int check_special(const char*);
void stopgap(const char*); //calls system shell [TEMP]


#ifndef GNU
	#ifndef TINY
		/*
		void completion(const char inpt[], linenoiseCompletions *lc){
		//Tab completion for linenoise lib
		//TODO maybe get all files in /bin/ /sbin/ /usr/bin/ and history?
			switch(inpt[0]){
			case 'c':
				linenoiseAddCompletion(lc, "cd");
				break;
			case 'e':
				linenoiseAddCompletion(lc, "echo");
				break;
			case 'h':
				linenoiseAddCompletion(lc, "help");
				break;
			case 'l':
				linenoiseAddCompletion(lc, "ls");
				linenoiseAddCompletion(lc, "lsblk");
				break;
			}
		}
	
		char *hints(const char inpt[], int *color, int *bold){
		//Creates a hint in purple
			*color = 35;
			*bold = 0;
			if(!strcmp("h", inpt))
				return "elp";
			else if(!strcmp("ls", inpt))
				return "blk";
			else
				return NULL;
		} */
	#endif
#endif

