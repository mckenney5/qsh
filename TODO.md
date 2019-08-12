# TODO
In order of importance  

## General:
- [ ] create a road map
- [X] make command history be located in the home dir (ex ~/.qsh.history)
- [ ] set a max history size (when its hit, start overwriting from the top down)
- [ ] finish all 'TODO' tags in source code
- [ ] figure out cross platform stuff (make for linux first)
- [ ] clean up code
- [ ] add more comments
- [X] test other libc implementations (*musl*, uClibc, etc)
- [ ] test in cygwin
- [ ] write a manpage

## Features:
- [ ] add command line args
- [X] handle ctrl^C (return to prompt, instead of closing)
- [X] add '~' operator to point to the users home dir (and use it for prompt too)
- [ ] add '&' operator to spawn another process
- [ ] add '\*' operator for 'any;
- [ ] handle internal and env variables
- [ ] handle linux pipes
- [ ] tokenize special chars (&&, ;, etc) to run multiple commands at once
- [ ] handle output redirection (>>, <, >, etc)
- [ ] use history file to populate auto complete
- [ ] use current directory to populate auto complete
- [ ] add aliases
- [ ] add aliase options (ex DOS, bash, zsh, etc);
- [ ] add user defined aliases
- [ ] add scripting
- [ ] be a drop in replacement for `sh` (or something close to)
- [ ] make a windows version
- [ ] POSIX compliance

# Completed
List of completed items from previous releases

## General:
- [X] add info for GNU readline and author info on linenoise-mob in README
- [X] be *usable* as a default shell
- [X] document and fix known bugs
- [X] add to github account
- [X] pretty up the README (ex links and pictures)
- [X] prioritize TODO list
- [X] come up with a list of commands
- [X] copy over old read me
- [X] mark all other quick term projects as deprecated
- [X] move TODO to its own file
- [X] add a license (MIT or GPLv2)
- [X] add color to prompt
- [X] audit code for memory leaks
- [X] audit code for buffer overflows
- [X] create a make file
- [X] understand gcc -O option ([optimization](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html))
- [X] fix [this bug](https://github.com/antirez/linenoise/issues/158) in linenoise lib

## Features:
- [X] ~~basic features (ex run a command, change directories)~~
- [X] ~~add a line editing feature~~


