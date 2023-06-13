/* Reads in a textfile and returns the tokens (split from whitespace by default) in a vector */
#ifndef _USING_EASY_TOKEN_H
#define _USING_EASY_TOKEN_H

std::string read_file(const std::string file_name); //Put the entire file into memory (I am sure this won't be a problem)
std::vector<std::string> tokenize(std::string input, std::string delim = " \t\r\n"); //splits a string into a vector with a delim

#endif

