#ifndef _USING_QSH_CONFIG_H
#define _USING_QSH_CONGIF_H

/* Default Values */

#define VERSION_STAGE "alpha"
#define VERSION_NUM "0.0.5"
#define VERSION_STATUS "unstable"
#define REPO "(https://github.com/mckenney5/qsh)"

#define CREDITS "This program does line editing via linenoise-mob\nfound here (https://github.com/rain-1/linenoise-mob)\n\n"\
		"linenoise-mob source is licensed under the \nBSD 2-Clause and is a copyright of \nSalvatore Sanfilippo and Pieter Noordhuis"
	
#define VERSION VERSION_STAGE " " VERSION_NUM " " VERSION_STATUS

//Built in command line args
const std::string default_command_line_args = "-h or --help\t\thelp\n-e or --execute\t\t<commands to run>\n";

//Default prompt that qsh starts with
const std::string default_prompt = "--> ";

//search path for a program, in order, must be NULL terminated and end in '/'
const char *default_search_path[] = {"/bin/", "/usr/sbin/", "./", NULL};

//The home dir
const std::string default_home_dir = "/home/";

#endif

