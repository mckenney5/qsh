/* This header lists the help for Quick Shell and all internal commands */
/* If you added a new command, put it in here */

#include <stdio.h>

#define VERSION_STAGE "alpha"
#define VERSION_NUM "0.0.1"
#define VERSION_STATUS "unstable"

#define VERSION VERSION_STAGE " " VERSION_NUM " " VERSION_STATUS

void help(); //displays all help information
void command_args(); //list all command args and their uses
void commands(); //lists all internal commands and their uses
void default_aliases(); //lists all default internal command aliases

void help(){
	puts("qsh - a small, lightweight command interpreter");
	printf("Version: %s\n", VERSION);
	puts("Command line args:");
	command_args();
	puts("Internal commands:");
	commands();
	puts("Default aliases");
	default_aliases();
	puts(" ");
}

void command_args(){
	puts("\tNone.");
}

void commands(){
	//list commands in alphabetical order
	//only internal commands should be noted in here
	//example: \tcls - clears the screen
	puts("\tcd\n\t\tChanges current directory");
	puts("\thelp\n\t\tDisplays this stuff");
	puts("\tquit\n\t\tQuits the program");
}
void default_aliases(){
	puts("\tNone.");
}
