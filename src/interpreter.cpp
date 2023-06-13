/* -- Command Interpreter --
 * Interfaces with a UI via the interpret(string command) and get_prompt()
 * interpret returns true if successful, false on error. get_prompt returns the current prompt string
 */

#include <iostream>
#include <vector>
#include <unordered_map>

/* C */
#include <stdio.h> //printf, puts
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h> //PATH_MAX
#include <pwd.h> //get username
#include <sys/wait.h> //waitpid
#include <cstring> //strlen, strcpy

/* Local */
#include "config.h"
#include "libs/easy_token.h"

using namespace std;

/* Globals */
bool interp_error_flag = false;

class Variables {
//internal variable database

	/* Private */
	unordered_map<string,string> db = { {"prompt", default_prompt} };

	/* Public */
	//list
	public: string list(){
		string result = "";
		for(auto& i: db){
			result.append(i.first + "='" + i.second + "'\n");
		}
		return result;
	}

	//search
	public: bool search(const string key){
		if(auto search = db.find(key); search != db.end())
			return true;
		else
			return false;
	}

	//add
	public: void add(const string key, const string value){
		if(search(key))
			db[key] = value;
		else
			db.insert({key, value});
	}

	//remove
	public: bool remove(const string key){
		if(search(key)){
			db.erase(key);
			return true;
		} else
			return false;
	}

	//get
	public: string get(const string key){
		if(auto search = db.find(key); search != db.end())
			return search->second;
		else
			return "";
	}

} vars;

int call(const char *full_path, char *argv[], bool wait = true){	
//Does the forking and calling work for running a program
	//pid_t parent = getpid();
	pid_t pid = fork();
	int status = 0;
	if (pid == -1){
		perror("Failed to fork program");
		interp_error_flag = true;
	} else if (pid > 0){
		if(wait) waitpid(pid, &status, 0);
	} else {
		execv(full_path, argv);
		_exit(EXIT_FAILURE);   // exec never returns
	};
	return status;
}

string search_file(const string file_name){
//Looks for a program in the specified locations in config.h
	string fullname = "";
	for(size_t i = 0; default_search_path[i] != NULL; i++){
		fullname = default_search_path[i] + file_name;
		if(access(fullname.c_str(), X_OK) == 0) //if we can execute it
			return default_search_path[i];
	}
	return "";
}

string get_prompt(){
// Gives the UI the set prompt
	if(vars.search("prompt"))
		return vars.get("prompt");
	else
		return default_prompt;
}

bool error_occured(){
// Checks if we had an error, then unsets the flag if true
	if(interp_error_flag == true){
		interp_error_flag = false;
		return true;
	} else
		return false;
}

void prompt(const string cmd){
// DEPRECATED TODO remove this since we can set variables
	if(cmd.size() < 7){
		cerr << "Prompt size too small" << endl;
	} else {
		string prompt = cmd.substr(7, cmd.size());
		vars.add("prompt", prompt);
	}

}

string get_cwd(){
// Get our current working directory
	char buf[PATH_MAX+1] = {'\0'};
	getcwd(buf, PATH_MAX);
	string cwd = buf;
	return cwd;
}

string get_user(){
// Gets the user's name
	struct passwd *pass;
	pass = getpwuid(getuid());
	string user = pass->pw_name;
	return user;
}

void set_cwd(string location){
// Sets the current working directory
	if(location == "") set_cwd(default_home_dir + get_user()); //if it is blank, go home
	if(chdir(location.c_str()) == -1){
		int status = errno;
		if(status == ENOENT){
			cerr << "Path not found." << endl;
			interp_error_flag = true;
		} else if(status == ENOTDIR){
			//if the path is not a dir then
			//cd into the folder its in (like zsh does)
			if(location[location.size()] == '/') location.pop_back();
			for(size_t i = location.size(); location[i] != '/' && i > 0; i++)
				location.pop_back();
			if(location.size() == 0){
				//if we cant fix the users command
				cerr << "Path not found." << endl;
				interp_error_flag = true;
			} else {
				if(chdir(location.c_str()) != 0){
					perror("Error (while fixing) ");
					interp_error_flag = true;
				}
			}
		} else {
			perror("Error ");
			interp_error_flag = true;
		}
	}
}

void help(){
	puts("qsh - a small, lightweight command interpreter");
	printf("Version: %s\n", VERSION);
	puts("======================================================");
	printf("The source code of this program is available here\n%s\n\n%s\n", REPO, CREDITS);
	puts("======================================================\n");
	puts("Command line args:");
	cout << default_command_line_args << endl;
	//puts("\nInternal commands:");
	//cout << default_commands << endl;
	//puts("\nDefault aliases");
	//default_aliases();
	puts(" ");
}


bool clean_input(string &cmd){
// Fixes input before running it

	//handle variables
	for(size_t location = cmd.find("$"); location != cmd.npos; location = cmd.find("$")){
		if(location == 0 || (cmd[location-1] != '\\' && location != cmd.size())){
			//if we find a non-escaped $, look up the var and replace it
			size_t end_location = cmd.find(" ", location); //what happens with just "$ "
			if(end_location == cmd.npos) end_location = cmd.size();
			cmd.replace(location, end_location-location, vars.get(cmd.substr(location+1, end_location)));
		}
	}
	
	//handle ~ meaning home
	for(size_t location = cmd.find("~"); location != cmd.npos; location = cmd.find("~")){
		if(location == 0 || (cmd[location-1] != '\\' && location != cmd.size())){
			//if we find a non-escaped $, look up the var and replace it
			cmd.replace(location, 1, default_home_dir + get_user());
		}
	}

	//handle  > < & &&
	if(cmd.find('&') != cmd.npos || cmd.find('|') != cmd.npos || cmd.find('>') != cmd.npos || cmd.find('<') != cmd.npos){
		system(cmd.c_str()); //TODO Replace this with own functions
		return false;
	}
	return true;
}

bool internal_commands(const string cmd, vector<string> t){
// Handles internal state of the program	
	if(t[0] == "exit")
		exit(0);
	else if(t[0] == "list")
		cout << vars.list() << endl;
	else if(t[0] == "prompt")
		prompt(cmd);
	else if(t[0].find('=') != t[0].npos) //if we are assigning a variable
		vars.add(cmd.substr(0, cmd.find('=')), cmd.substr(cmd.find('=')+1, cmd.size()));
	else if(t[0] == "cd")
		set_cwd(t[1]); //TODO add a handle for quoted args
	else if(t[0] == "help")
		help();
	else
		return false;
	return true; //if we ran a command

}

bool interpret(string cmd){
// Handles input. Checks if it is an internal command or a file
	if(cmd == "") return false;

	if(!clean_input(cmd)) return true; //if we ran a command in clean_input then we are done, this is temporary

	vector<string> t = tokenize(cmd);
	
	if(internal_commands(cmd, t)) //if it is an internal oommand, run it
		return true;

	//if it is not an internal command, check if it is a file, then run it.

	string location = search_file(t[0]);
	if(location == ""){
		cerr << "Program '" << t[0] << "' is not found!" << endl;
		interp_error_flag = true;
	} else {
		location.append(t[0]);
		
		//Convert a vector to a C pointer array, dynamically
		char** argv = new char*[t.size()+1];
		for(size_t i = 0; i < t.size(); i++)
			argv[i] = strdup(t[i].c_str());
		argv[t.size()] = NULL; //null terminate the list

		call(location.c_str(), argv, t.size());
		
		//free argv since we used the 'new' keyword
		for(size_t i = 0; i < t.size(); i++)
			free(argv[i]);
		delete[] argv; 
	}
	return !error_occured(); //lets the UI know if we finished the command successfully or not
}


