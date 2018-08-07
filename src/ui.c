/* Quick Shell's main file */

#include "ui.h"

int main(void){
	char inpt[MAX_USER_INPUT]; //user input
	char prompt[MAX_USER_INPUT]; //prompt to display
	strncpy(prompt, DEFAULT_PROMPT, MAX_USER_INPUT);
	char hostname[255] = {'\0'}; //computer hostname for prompt
	char cdir[255] = {'\0'}; //current working directory for prompt
	char *user; //user for prompt
	struct passwd *pass; //structure to grab user information for prompt
	#ifndef GNU
		#ifndef TINY
			//sets up autocompletion, hints, and a history for linenoise lib
			linenoiseSetCompletionCallback(completion);
			linenoiseSetHintsCallback(hints);
			linenoiseHistoryLoad(HIST_FILE);
			char *input;
		#endif
	#endif
	
	while(1){
		//Clear user input
		memset(inpt, '\0', MAX_USER_INPUT);
		
		//get the hostname of the computer
		gethostname(hostname, sizeof(hostname));

		//get the current working directory
		getcwd(cdir, sizeof(cdir));

		//get the username from the system
		pass = getpwuid(getuid());
		user = pass->pw_name;

		//Display prompt
		if(!strcmp("root", user))
			snprintf(prompt, MAX_USER_INPUT,
			"%s%s%s@%s%s:%s%s%s# ", RED, user, WHITE, RED, 
			hostname, BLUE, cdir, RESET);	
		else
			snprintf(prompt, MAX_USER_INPUT, 
			"%s%s%s@%s%s:%s%s%s$ ", WHITE, user, RED, WHITE, 
			hostname, BLUE, cdir, RESET);
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
		if(!strcmp("quit", inpt) || !strcmp("exit", inpt)) break;
		if(!strcmp("rebuild", inpt) || !strcmp("r", inpt)){
			if(DEBUG){
				//allows the dev to easily recompile the program
				system("clear && ./compile.sh && ./qt && rm qt");
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
					free(input);
				#endif
			#endif
		}
	}
	return 0;
}

void interp(char inpt[]){
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
		perror("Error in program: NULL pointer in interp");
		return;
	}
	char *program = calloc(sizeof(char), MAX_USER_INPUT);
	if(find(token, only_local, &program)){
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

	} else {
		puts(BAD_COMMAND);
	}
	free(program);
}

int find(const char inpt[], const int only_local, char *location[]){
	if(only_local){
		if(access(inpt, F_OK) != -1){
			strncpy(*location, inpt, MAX_USER_INPUT);
			return 1;	
		} else return 0;
	}

	char temp[MAX_USER_INPUT*2] = {'\0'};

	strncpy(temp, DEFAULT_EXEC_PATH1, MAX_USER_INPUT*2);
	strncat(temp, inpt, MAX_USER_INPUT*2);
	if(access(temp, F_OK) != -1){ strncpy(*location, temp, MAX_USER_INPUT); return 1; }
	
	memset(temp, '\0', MAX_USER_INPUT*2);
	strncpy(temp, DEFAULT_EXEC_PATH2, MAX_USER_INPUT*2);
	strncat(temp, inpt, MAX_USER_INPUT*2);
	if(access(temp, F_OK) != -1){ strncpy(*location, temp, MAX_USER_INPUT); return 1; }
	
	memset(temp, '\0', MAX_USER_INPUT*2);
	strncpy(temp, DEFAULT_EXEC_PATH3, MAX_USER_INPUT*2);
	strncat(temp, inpt, MAX_USER_INPUT*2);
	if(access(temp, F_OK) != -1){ strncpy(*location, temp, MAX_USER_INPUT);  return 1; }

	return 0;

}

int handle_var(char **inpt){
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

void run(const char *location, const char * program, char* args){
	//Takes the location and argv (program name then args)
	//pid_t parent = getpid();
	pid_t pid = fork();
	char *args2[MAX_ARGS+1];
	unsigned int i;
	unsigned argc = 0;
	args2[0] = calloc(sizeof(char), MAX_USER_INPUT);

	if(args == NULL)
		//if no args were supplied
		args2[0][0] = '\0';
	else {
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

	if (pid == -1){
		perror("Failed to fork program");
	} else if (pid > 0){
		//TODO check return status?
		int status;
		waitpid(pid, &status, 0);
	} else {
		if(args2[0][0] != '\0'){
			//if there were args, call execv with '*argv[]'
			execv(location, args2);
		} else
			//if no args were supplied then
			//supply the program name as 'argv[0]'
			//remember that 'argv[0]' is always the program name
			execl(location, program, (char*) NULL);
		_exit(EXIT_FAILURE);   // exec never returns
	};
	for(i=0; i < argc; i++) //frees all alocated memory
		free(args2[i]);
}
