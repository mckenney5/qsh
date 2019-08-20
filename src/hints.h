#ifndef _using_hints_h
#define _using_hints_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include "libs/linenoise/linenoise.h"
#include "libs/linux.h"
#include "ui.h"

//#define DIR_SIZE 1024
#define DIR_MAX 100
#define HIST_FILE "/.qsh.history"
#define VAR_SIZE 255
#define MAX_USER_INPUT 1024

int get_hist(char* hist_file, size_t size){
	char home[VAR_SIZE] = {'\0'};
	//gets home dir
	get_home(home, sizeof(home));

	//sets history file location
	strncpy(hist_file, home, size);
	strcat(hist_file, HIST_FILE);
	if(linenoiseHistoryLoad(hist_file) == -1){
		perror("qsh : Error loading history file");
		return 1;
	}
	return 0;
}

int list_dir(char dir_info[][NAME_MAX]){
/* creates a list of dirs in the cwd */
	char cwd[NAME_MAX] = {'\0'};
	int i=0;
	DIR *d;
	struct dirent *dir;
	d = opendir(get_cwd(cwd, NAME_MAX));
	if(d){
		while((dir = readdir(d)) != NULL && i < DIR_MAX){
			if(dir->d_type == DT_DIR){
				strcpy(dir_info[i], dir->d_name);
				i++;
			}
		}
		closedir(d);
		return 0;
	}
	return 1;
}

void completion(const char *buf, linenoiseCompletions *lc) {
	char dir_info[DIR_MAX][NAME_MAX] = {'\0'};
	int i;
	if(!strcmp(buf, "cd") && list_dir(dir_info) != 1){
		char command[NAME_MAX+3] = {'\0'};
		for(i=0; i < DIR_MAX && dir_info[i] != NULL; i++){
			strcpy(command, "cd ");
			strcat(command, dir_info[i]);
			linenoiseAddCompletion(lc, command);
		}
	} else if(buf[0] == '\0'){
		//load and suggest commands from history
		char hist_file[VAR_SIZE] = {'\0'};
		get_hist(hist_file, sizeof(hist_file));	
		FILE *fp;
		fp = fopen(hist_file, "r");
		if(fp == NULL){
			perror("qsh : Error suggesting hist file");
			return;
		}
		char input[MAX_USER_INPUT] = {'\0'};
		int lines = 0;
		char output = fgetc(fp);
		while(output != EOF){
			if(output == '\n')
				lines++;
			output = fgetc(fp);
		}
		fclose(fp);
		fp = fopen(hist_file, "r");
		for(i=0; i < lines; i++){
			fgets(input, MAX_USER_INPUT, fp);
			input[strlen(input)-1] = '\0';
			//printf("'%s'\n", input);
			linenoiseAddCompletion(lc, input);
		}
		fclose(fp);
	
	} else {

		switch(buf[0]){
			case 'e':
				linenoiseAddCompletion(lc, "exit");
				linenoiseAddCompletion(lc, "end");
				break;
			case 'h':
				linenoiseAddCompletion(lc, "help");
				break;
			case 'q':
				linenoiseAddCompletion(lc, "quit");
				break;
		}
	}
}

char *hints(const char *buf, int *color, int *bold){
	return NULL;
/* Disabled
	*color = 35;
	*bold = 0;
	switch(buf[0]){
		case 'c':
			return (char *)"d ";
			break;
		case 'e':
			return (char *) "xit";
			break;
		case 'h':
			return (char *) "elp";
			break;
		case 'q':
			return (char *) "uit";
			break;
		default:
			return NULL;
	} */
	/*
	if (!strcasecmp(buf,"hello"))
        	return (char *)" World";
    	else
		return NULL;
	*/
}
#endif

