/* Reads in a textfile and returns the tokens (split from whitespace by default) in a vector */
#include <iostream> //cout, cerr, getline
#include <fstream> //ifsteam, open
#include <vector> //vectors

using namespace std;

string read_file(const string file_name){
	//Put the entire file into memory (I am sure this won't be a problem)
	string entire_file = "";

	ifstream text_file;
	text_file.open(file_name);
	if(!text_file){
		//cerr << "Cannot open file '" << file_name << "'! " << endl;
		//perror("Additional Info: ");
	} else
		getline(text_file, entire_file, '\0');
	return entire_file;
}
vector<string> tokenize(string input, string delim = " \t\r\n"){
	vector<string> tokens;
	string word = "";
	bool found = false;

	for(size_t i = 0; input[i] != '\0'; i++){
		//split string by delim
		for(size_t l = 0; delim[l] != '\0'; l++){
			if(input[i] == delim[l]){
				if(word != "") tokens.push_back(word);
				word = "";
				found = true;
				break;
			}
		}
		if(!found)
			word.push_back(input[i]);
		else 
			found = false;
	}
	if(word != "") tokens.push_back(word); //adds the last word
	tokens.shrink_to_fit(); //shrink vector since we should not need to add more data
	return tokens;
}

/*
int main(int argc, char *argv[]){
	//testing script - treats args as either a textfile or string

	//testing script:
	size_t total_tokens = 0;	
	string file_name = "test.txt";
	string data = "";
	vector<string> tokens;

	if(argc == 1){
		data = read_file(file_name);
		cout << "Data: \n'" << data << "'" << endl;
		tokens = tokenize(file_name);

		cout << "Tokens: " << endl;
		for(size_t i = 0; i < tokens.size(); i++, total_tokens++)
			cout << "'" << tokens[i] << "'" << endl;
	} else {
		for(size_t i = 1; i < argc; i++){
			cout << argv[i] << ":" << endl;
			data = read_file(argv[i]);
			if(data == "")
				tokens = tokenize(argv[i]);
			else {
				cout << "Data: \n'" << data << "'" << endl;
				tokens = tokenize(data);
			}
			cout << "Tokens: " << endl;
			for(size_t i = 0; i < tokens.size(); i++, total_tokens++)
				cout << "'" << tokens[i] << "'" << endl;
			cout << "---------------" << endl;
		}
	}
	cout << "Total Tokens: " << total_tokens << endl;
	return 0;
}
*/

