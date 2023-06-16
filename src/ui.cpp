/* -- QSH's Main File --
 * This file handles the user interface between the interpreter (interpeter.cpp) and the user
 * It is the UI's job to:
 * - not let blank strings / NULL strings pass as input (this saves boiler plate)
 * - handle interupts (Ctrl+C)
 * - take and pass command line args to interpret (NOTE: not implemented yet)
 * - (optional) maintain a command history
 * - (optional) have line editing capabilites (see gnu-readline / linenoise)
 */

#include <iostream>
#include <vector>

/* C */
#include <signal.h> //handle interupts

/* Local */
#include "interpreter.h"
#include "libs/linenoise/linenoise.h"

using namespace std;

int ui(){
// Handles the basic UI for the shell

	//Ignore signals sent to us
	//According to signal(2) this is portable
	signal(SIGINT, SIG_IGN);
	sigset_t mask;
	sigfillset(&mask);
	sigprocmask(SIG_SETMASK, &mask, NULL);

	char *ptr = NULL;

	while(true){
		//get input
		string cmd = "";
		ptr = linenoise(get_prompt().c_str()); //a pointer needs to be here to be free'd since linenoise uses strdup for input
		if(ptr == NULL) return 1;
		cmd = ptr;

		if(cmd != "" && interpret(cmd) == true)
			linenoiseHistoryAdd(cmd.c_str()); //add to history
							  //
		linenoiseFree(ptr); //uses free() built in to linenoise
	}
	return 0;
}

int main(int argc, char *argv[]){
	if(argc > 3){
		cerr << "Error: too many args. Try --help" << endl;
		return 1;
	}
	if(argc == 1) return ui(); //if no args, go to interactive mode

	string arg = argv[1];

	if(arg == "--help" || arg == "-h")
		interpret("help");
	else if((arg == "--execute" || arg == "-e") && argc == 3 && argv[2] != NULL)
		interpret(arg = argv[2]);
	else if(argc == 1)
		return ui();
	else {
		cerr << "Error: invalid argument." << endl;
		interpret("help");
		return 1;
	}
		
	return 0;
}

