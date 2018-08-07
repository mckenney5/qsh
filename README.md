Quick Shell
==============
(qsh) A small, lightweight, command interpreter

#Description
This is the rewrite of quick terminal, quick terminal server edition, and QTerminal. 
Quick shell aims at replacing bash or zsh with a smaller and lighter program and follows
the suckless philosophy (https://suckless.org/philosophy/). (note this project is not
associated with suckless).

#Compiling and Testing
This program allows a couple of compiling options.

The simplest way is to compile by `make`

Make Options:
* `make GNU`	: makes the user interface use GNU readline
* `make TINY`	: makes the UI only use fgets (aka no line editing, like sh(1))
* `make DEBUG`	: adds debugging symbols to the program, along with verbose output
* `make`	: makes the UI use linenoise libray (https://github.com/antirez/linenoise)

(optional) Colors:
* NO_COLOR (-DNO_COLOR)	: disables color in the prompt ONLY

#How to contribute
Fork the repository, compile, test, make changes, test, then submit a merge/pull request

