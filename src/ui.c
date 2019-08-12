/* Quick Shell's main file */

#include "ui.h"

int main(int argc, char *argv[]){
	if(DEBUG) printf("called with '%d' args\n", argc);
	if(argc == 1)
		return ui();
	
	if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
		help();
	else if(!strcmp(argv[1], "-r") || !strcmp(argv[1], "--run")){
		if(argc == 3)
			interp(argv[2]);
		else
			perror("qsh : Missing args, syntax qsh -r <command>");
	} else
		help();
	return 0;
}

int ui(void){
	//Ignore signals sent to us
	//According to signal(2) this is portable
	signal(SIGINT, SIG_IGN);
	sigset_t mask;
	sigfillset(&mask);
	sigprocmask(SIG_SETMASK, &mask, NULL);

	//keeps tally of all the NULLs that the CLI takes
	unsigned int bad_call = 0;

	char inpt[MAX_USER_INPUT] = {'\0'}; //user input
	char prompt[MAX_USER_INPUT] = {'\0'}; //prompt to display
	strncpy2(prompt, DEFAULT_PROMPT, MAX_USER_INPUT);
	char hostname[VAR_SIZE] = {'\0'}; //computer hostname for prompt
	char cdir[VAR_SIZE] = {'\0'}; //current working directory for prompt
	char user[VAR_SIZE] = {'\0'}; //current user name
	char old_user[VAR_SIZE] = {'\0'}; //used to check if the user changed
	char last[VAR_SIZE] = {'\0'}; //dir we are in
	char home[VAR_SIZE] = {'\0'}; //holds home dir
	char hist_file[VAR_SIZE] = {'\0'}; //name and location of the history file
	
	//gets history file location and loads it
	get_hist(hist_file, sizeof(hist_file));

	#ifndef TINY
		#ifndef GNU
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

		//gets home dir
		get_home(home, sizeof(home));

		//get the dir we are in, if its home, use ~
		get_cwd_last(last, sizeof(last));
		if(!strcmp(home, cdir)) strncpy2(last, "~", sizeof(last));

		//get the username from the system and checks for changes
		get_user(user, sizeof(user));
		if(old_user[0] == '\0'){
			strncpy2(old_user, user, VAR_SIZE);
		} else if(strcmp(old_user, user)){
			strncpy2(old_user, user, VAR_SIZE);
			get_hist(hist_file, VAR_SIZE);
		}
	
		//Display prompt
		if(!strcmp("root", user))
			snprintf(prompt, MAX_USER_INPUT,
			"%s%s%s@%s%s:%s%s%s# ", RED, user, WHITE, RED, 
			hostname, CYAN, cdir, RESET);	
		else
			snprintf(prompt, MAX_USER_INPUT, 
			"%s%s%s@%s%s:%s%s%s$ ", WHITE, user, RED, WHITE, 
			hostname, CYAN, last, RESET);

		#ifdef TINY
			//get and clean the input
			printf(prompt);
			fgets(inpt, MAX_USER_INPUT, stdin);
			inpt[strlen(inpt)-1] = '\0'; //removes the newline char
		#else
			#ifdef GNU
				//get user input
				strncpy2(inpt, readline(prompt), MAX_USER_INPUT);
			#else	
				input = linenoise(prompt);
				//strncpy2(input, linenoise(prompt), MAX_USER_INPUT);
				if(input != NULL)
					strncpy2(inpt, input, MAX_USER_INPUT);
				else {
					inpt[0] = '\0';
				}
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
			bad_call = 0; //since the user still has control, reset
			interp(inpt);

			#ifdef GNU
				add_history(inpt);
			#else
				#ifndef TINY
					linenoiseHistoryAdd(inpt);
					linenoiseHistorySave(hist_file);
				#endif
			#endif
		} else
			if(++bad_call >= 30) exit(2); //stops prompt from spamming

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
	has_home(inpt);
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
		strncpy2(token_inpt, inpt, MAX_USER_INPUT);

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
			char name[MAX_USER_INPUT] = {'\0'};
			strncpy2(name, token, MAX_USER_INPUT);
			char args[MAX_USER_INPUT] = {'\0'};
	
			//checks if command has args
			int has_args = 0;
			for(i=0; inpt[i] != '\0'; i++)
				if(inpt[i] == ' ') has_args = 1;
			
			//remove the name from the args
			if(has_args){
				for(i=0; inpt[strlen(name)+i+1] != '\0'; i++)
					args[i] = inpt[strlen(name)+i+1];
			}
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
			if(location != NULL) strncpy2(*location, inpt, MAX_USER_INPUT);
			return 1;	
		} else return 0;
	}

	char temp[MAX_USER_INPUT*2] = {'\0'};

	strncpy2(temp, DEFAULT_EXEC_PATH1, MAX_USER_INPUT*2);
	strcat(temp, inpt);
	if(access(temp, X_OK) != -1){ if(location != NULL) strncpy2(*location, temp, MAX_USER_INPUT); return 1; }
	
	memset(temp, '\0', MAX_USER_INPUT*2);
	strncpy2(temp, DEFAULT_EXEC_PATH2, MAX_USER_INPUT*2);
	strcat(temp, inpt);
	if(access(temp, X_OK) != -1){ if(location != NULL) strncpy2(*location, temp, MAX_USER_INPUT); return 1; }
	
	memset(temp, '\0', MAX_USER_INPUT*2);
	strncpy2(temp, DEFAULT_EXEC_PATH3, MAX_USER_INPUT*2);
	strcat(temp, inpt);
	if(access(temp, X_OK) != -1){ if(location != NULL) strncpy2(*location, temp, MAX_USER_INPUT);  return 1; }

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
		strncpy2(var, token, sizeof(var));
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
		strncpy2(input, *inpt, strlen(*inpt));
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
		strncpy2(temp, *inpt, MAX_USER_INPUT);
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
		strncpy2(args2[0], program, MAX_USER_INPUT);
		for(i=1; i < MAX_ARGS && token != NULL; i++){
			//Alocates the mem for arg, puts the token in
			//And sets the next pointer to NULL as required
			//by execv
			args2[i] = calloc(sizeof(char), MAX_USER_INPUT);
			strncpy2(args2[i], token, MAX_USER_INPUT);
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
	return 1;
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

int get_hist(char* hist_file, size_t size){
	char home[VAR_SIZE] = {'\0'};
	#ifndef GNU
		#ifndef TINY
			//sets up autocompletion, hints, and a history for linenoise lib
			//linenoiseSetCompletionCallback(completion);
			//linenoiseSetHintsCallback(hints);
			
			//gets home dir
			get_home(home, sizeof(home));

			//sets history file location
			strncpy(hist_file, home, size);
			strcat(hist_file, HIST_FILE);
			if(linenoiseHistoryLoad(hist_file) == -1) puts("qsh: Error loading history file");
			return 0;
		#endif
	#endif
	return 1;
}

char* has_home(char inpt[]){
	int c, a, b = 0; //TODO rewrite vars to be more readable
	int replaced = 0;
	char input[MAX_USER_INPUT] = {'\0'};
	for(c=0; c < MAX_USER_INPUT && inpt[c] != '\0'; c++){
		if(inpt[c] == '~' && inpt[c+1] == '/'){
			replaced = 1;
			for(a=0; a < MAX_USER_INPUT && inpt[a] != '\0'; a++){
				if(a != c){
					input[b] = inpt[a];
					b++;
				} else {
					//add home and add that to b
					char home[VAR_SIZE] = {'\0'};
					get_home(home, VAR_SIZE);
					if(strlen(inpt) + strlen(home) >= MAX_USER_INPUT) perror("qsh : Error user input too big");
					strcat(input, home);
					b += strlen(home);
					input[b++] = '/';
					a = c+1;
				}
			}
		}
		
	}
	if(replaced == 0)
		NULL;
	else {
		strncpy2(inpt, input, MAX_USER_INPUT);
	}
	return inpt;
}

