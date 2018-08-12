/* Quick Shell's main file */

#include "ui.h"
int main(void){
	char inpt[MAX_USER_INPUT]; //user input
	char prompt[MAX_USER_INPUT]; //prompt to display
	strncpy(prompt, DEFAULT_PROMPT, MAX_USER_INPUT);
	char hostname[255] = {'\0'}; //computer hostname for prompt
	char cdir[255] = {'\0'}; //current working directory for prompt
	char user[255] = {'\0'}; //current user name

	#ifndef GNU
		#ifndef TINY
			//sets up autocompletion, hints, and a history for linenoise lib
			//linenoiseSetCompletionCallback(completion);
			//linenoiseSetHintsCallback(hints);
			linenoiseHistoryLoad(HIST_FILE);
			char *input=NULL;
		#endif
	#endif
	
	while(1){
		//Clear user input
		memset(inpt, '\0', MAX_USER_INPUT);
		
		//get the hostname of the computer
		get_hostname(hostname, sizeof(hostname));

		//get the current working directory
		get_cwd(cdir, sizeof(cdir));

		//get the username from the system
		get_user(user, sizeof(user));

		//Display prompt
		if(!strcmp("root", user))
			snprintf(prompt, MAX_USER_INPUT,
			"%s%s%s@%s%s:%s%s%s# ", RED, user, WHITE, RED, 
			hostname, CYAN, cdir, RESET);	
		else
			snprintf(prompt, MAX_USER_INPUT, 
			"%s%s%s@%s%s:%s%s%s$ ", WHITE, user, RED, WHITE, 
			hostname, CYAN, cdir, RESET);

		#ifdef TINY
			//get and clean the input
			printf(prompt);
			fgets(inpt, MAX_USER_INPUT, stdin);
			inpt[strlen(inpt)-1] = '\0'; //removes the newline char
		#else
			#ifdef GNU
				//get user input
				strncpy(inpt, readline(prompt), MAX_USER_INPUT);
			#else	
				input = linenoise(prompt);
				strncpy(inpt, input, MAX_USER_INPUT);
			#endif
		#endif

		//check if the input is quit or rebuild, if not then check the commands
		if(!strcmp("quit", inpt) || !strcmp("exit", inpt)){
			#ifndef GNU
				#ifndef TINY
					free(input);
				#endif
			#endif
			 break;
		}
		if(!strcmp("rebuild", inpt) || !strcmp("r", inpt)){
			if(DEBUG){
				//allows the dev to easily recompile the program
				system("clear && make DEBUG && ./qsh.debugging && rm qsh.debugging");
				memset(inpt, '\0', MAX_USER_INPUT);
			}
		}
		if(strcmp("\0", inpt)){ //if the inpt is NULL then skip
			interp(inpt);
			#ifdef GNU
				add_history(inpt);
			#else
				#ifndef TINY
					linenoiseHistoryAdd(inpt);
					linenoiseHistorySave(HIST_FILE);
				#endif
			#endif
		}
		#ifndef GNU
			#ifndef TINY
				free(input);
				input = NULL;
			#endif
		#endif

	}
	return 0;
}

static void interp(char inpt[]){
/* This should first check if the first char is a command */
	int status=1;
	switch (inpt[0]){
		case 'c':
			status = cCommands(inpt);
			break;
		case 'h':
			if(!strcmp("help", inpt)){ 
				help(); //special case since help should always be internal
				status=0;
			} else NULL; //goto h commands
			break;
		case '$':
			//vars
			status = handle_var(&inpt);
			//interp(inpt);
			//status = 0;
			break;
	}
	if(status == 0) return; //if the command ran without issue
/* TODO split this into multiple functions */
	//check to see if the user is trying to set a var
	//if so then handle it, if the = is escaped '\=' ignore
	char *isVar = strstr(inpt, "=");
	if(isVar != NULL){
		--isVar;
		if(isVar[0] != '\\'){
			handle_var(&inpt);
			return;
		}
	}
	//check if the command has special chars and handle it (ex & and |)
	if(check_special(inpt)) return;

	//since the input is not a command, look for the file
	char *token;
	char token_inpt[MAX_USER_INPUT] = {'\0'}; 
	int only_local = 0;
	int i, l=0;
	if(strlen(inpt) > 2 && inpt[0] == '.' && inpt[1] == '/'){
		only_local = 1; //look only for local programs
		//int i, l=0;
		for(i=2; i < MAX_USER_INPUT; i++){
			token_inpt[l] = inpt[i];
			l++;
		}
	} else
		strncpy(token_inpt, inpt, MAX_USER_INPUT);

	token = strtok(token_inpt, " ");
	if(token == NULL){
		//only gets called if the input is only spaces ' '
		//treat like no input
		//perror("Error in program: NULL pointer in interp");
		return;
	}

	if(find(token, only_local, NULL)){
		char *program = calloc(sizeof(char), MAX_USER_INPUT);
		find(token, only_local, &program);
		if(isFile(program)){
			//if the file exists
			//get the name of the program
			char name[MAX_USER_INPUT];
			strncpy(name, token, MAX_USER_INPUT);
			char args[MAX_USER_INPUT] = {'\0'};
	
			//remove the name from the args
			for(i=0; inpt[strlen(name)+i+1] != '\0'; i++)
				args[i] = inpt[strlen(name)+i+1];
	
			//run the program
			run(program, name, args);

		} else
			puts("qsh : That is not a file.");
		//Free mem
		free(program);

	} else {
		puts(BAD_COMMAND);
	}
}

static int find(const char inpt[], const int only_local, char *location[]){
	if(only_local){
		if(access(inpt, X_OK) != -1){
			if(location != NULL) strncpy(*location, inpt, MAX_USER_INPUT);
			return 1;	
		} else return 0;
	}

	char temp[MAX_USER_INPUT*2] = {'\0'};

	strncpy(temp, DEFAULT_EXEC_PATH1, MAX_USER_INPUT*2);
	strncat(temp, inpt, MAX_USER_INPUT*2);
	if(access(temp, X_OK) != -1){ if(location != NULL) strncpy(*location, temp, MAX_USER_INPUT); return 1; }
	
	memset(temp, '\0', MAX_USER_INPUT*2);
	strncpy(temp, DEFAULT_EXEC_PATH2, MAX_USER_INPUT*2);
	strncat(temp, inpt, MAX_USER_INPUT*2);
	if(access(temp, X_OK) != -1){ if(location != NULL) strncpy(*location, temp, MAX_USER_INPUT); return 1; }
	
	memset(temp, '\0', MAX_USER_INPUT*2);
	strncpy(temp, DEFAULT_EXEC_PATH3, MAX_USER_INPUT*2);
	strncat(temp, inpt, MAX_USER_INPUT*2);
	if(access(temp, X_OK) != -1){ if(location != NULL) strncpy(*location, temp, MAX_USER_INPUT);  return 1; }

	return 0;

}

static int handle_var(char **inpt){
//replaces inpt with dereffed vars
//printf("handle called: '%s'\n", *inpt);
	if(strchr(*inpt, '=') != NULL){
		char *token;
		token = strtok(*inpt, "=");
		if(token == NULL){
			perror("Error in program, NULL pointer in handle_var().");
			return 1;
		}
		char var[sizeof(token)];
		strncpy(var, token, sizeof(var));
		token = strtok(NULL, "=");
		if(token == NULL){
			puts("Var unset.");
			unsetenv(var);
		} else {
			if(setenv(var, token, 0) == -1)
				perror("Error setting var");
			puts("Var set.");
		}
		return 0;
	/*} else if(strchr(*inpt, '$') != NULL && strstr(*inpt, "\\$") == NULL){
		//puts("Soon (tm)");
		//return 1;
		//replace the var with its value
		*inpt = strchr(*inpt, '$');//DOES NOT REMOVE $!
		char *token;
		char input[strlen(*inpt)];
		strncpy(input, *inpt, strlen(*inpt));
		token = strtok(input, " ");
		if(token == NULL){
			perror("Error in program, NULL pointer in handle_var().");
			return 1;
		}
		char *value = getenv(token);
		if(value == NULL){ puts("Var not found"); return 1; }
		printf("HACK: Var='%s'\n", value); //HACK
		//char *new_value[MAX_USER_INPUT];
		//replace $var with value while making sure spaces and nulls are correct
		//vsprintf(new_value, "%s %s", value, token);
		return 0;
	*/
	} else if(*inpt[0] == '$') { //HACK: this should be changed when ^above is fixed
		int i, l=0;
		char var[MAX_USER_INPUT] = {'\0'};
		char temp[MAX_USER_INPUT];
		strncpy(temp, *inpt, MAX_USER_INPUT);
		for(i=1; i < MAX_USER_INPUT; i++){
			if(temp[i] == '\0') break;
			var[l] = temp[i];
			l++;
		}
		printf("Var '%s'\n", var);
		char *value = getenv(var);
		if(value == NULL){ puts("Var not found"); return 0; }
		printf("%s: '%s'\n", var, value);
		return 0;

	} else {
		puts("IRGNORED");
		return 1;
	}
}

static void run(const char *location, char *program, char* args){
	//converts user input into something exec can understand
	char *args2[MAX_ARGS+1];
	unsigned int i=0;
	unsigned argc = 0;
	if(args == NULL){
		//if no args were supplied
		args2[0] = program;
		args2[1] = NULL;
	} else {
		args2[0] = calloc(sizeof(char), MAX_USER_INPUT);
		char *token;
		//printf("args: '%s'\n", args);
		token = strtok(args, " ");
		strncpy(args2[0], program, MAX_USER_INPUT);
		for(i=1; i < MAX_ARGS && token != NULL; i++){
			//Alocates the mem for arg, puts the token in
			//And sets the next pointer to NULL as required
			//by execv
			args2[i] = calloc(sizeof(char), MAX_USER_INPUT);
			strncpy(args2[i], token, MAX_USER_INPUT);
			args2[i+1] = NULL;	
			token = strtok(NULL, " ");
			argc++;	
		}	
	}
	fflush(stdout); //clears any garbage from the UI
	call(location, program, args2, argc); //TODO pass by ref?
	if(i != 0){ //checks if args was alloc'd
		for(i=0; i <= argc; i++){ //frees and whipes all alocated memory
			memset(args2[i], '\0', sizeof(char)*MAX_USER_INPUT);
			free(args2[i]);
		}
	} else args2[0]=NULL;
}

static int call(const char *location, char *program, char *argv[], int argc){	
	//Does the forking and calling work
	//pid_t parent = getpid();
	pid_t pid = fork();
	int status = 0;
	if (pid == -1){
		perror("Failed to fork program");
	} else if (pid > 0){
		//TODO check return status?
		waitpid(pid, &status, 0);
	} else {
		if(argc >= 1 && argv[1] != NULL){
			//if there were args, call execv with '*argv[]'
			execv(location, argv);
		} else {
			//if no args were supplied then
			//supply the program name as 'argv[0]'
			//remember that 'argv[0]' is always the program name	
			execl(location, program, (char*) NULL);
		}
		_exit(EXIT_FAILURE);   // exec never returns
	};
	return status;
}

static int isFile(const char* path){
	struct stat buf;
	if(stat(path, &buf) == 0 && S_ISREG(buf.st_mode)) return 1;
	else return 0;
}

int check_special(const char* inpt){
	unsigned int i;
	for(i=0; inpt[i] != '\0'; i++){
		switch (inpt[i]){
			case '|':
				//handle pipes
				stopgap(inpt);
				return 1;
				break;
			case '&':
				if(inpt[i+1] == '&')
					//split commands
					NULL;
				else
					//handle fork
					stopgap(inpt);
				return 1;
				break;
		}
	}
	return 0; //didnt find anything
}

void stopgap(const char* inpt){
	system(inpt);
}

