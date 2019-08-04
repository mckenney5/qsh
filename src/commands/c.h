/* Commands that start with 'c' */
#include <stdio.h> //perror
#include <stdlib.h> //clear, calloc, free
#include <unistd.h> //chdir
#include <string.h> //strcmp, strlen, etc
#include <errno.h> //chdir errors
int cCommands(const char[]);

int cCommands(const char inpt[]){
	unsigned int i, size=0;
	for(i=0; inpt[i] != '\0'; i++) size++; //get string length
	char *input = calloc(size+1, sizeof(char));
	char *token;
	strncpy(input, inpt, size);
	token = strtok(input, " ");
	if(token == NULL || input == NULL){
		perror("qsh : Error in program, NULL pointer in cCommands().");
		free(input);
		return 1;
	}

	//Checks commands in alphabetical order
	if(!strcmp("cd", token) && strlen(inpt) > 3){
		char new_dir[FILENAME_MAX] = {'\0'};
		int l=0;
		for(i=3; i < strlen(inpt); i++){
			new_dir[l] = inpt[i];
			l++;
		}
		if(chdir(new_dir) == -1){
			int status = errno;
			if(status == ENOENT)
				puts("Path not found.");
			else if(status == ENOTDIR){
				//if the path is not a dir then
				//cd into the folder its in (like zsh does)
				int a;
				for(a=strlen(new_dir); a >= 0; a--){
					if(new_dir[a] == '/') break;
					new_dir[a] = '\0';
				}
				if(new_dir[0] == '\0')
					//if we cant fix the users command
					puts("Path is not a directory.");
				else {
					if(chdir(new_dir) != 0)
						perror("Error (while fixing) ");
				}
			} else
				perror("Error ");
		}
	} else {
		free(input);
		return 1;
	}
	free(input);
	return 0;
}

