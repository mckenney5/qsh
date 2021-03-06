# Road Map
List of minimum features before every release

## Alpha 0.0.5 [NEXT VERSION]
### General
- [ ] set a max history size (when its hit, start overwriting from the top down)
- [ ] update screenshot

### Features
- [ ] handle linux pipes
- [ ] handle output redirection (>>, <, >, etc)
- [ ] add '&' operator to spawn another process
- [ ] add '\*' operator for 'any;
- [ ] handle internal and env variables
- [ ] tokenize special chars (&&, ;, etc) to run multiple commands at once


## Beta 0.1.0
### General:
- [ ] finish all 'TODO' tags in source code
- [ ] clean up code
- [ ] add more comments
- [ ] squash all bugs
- [ ] signed .deb packages for release

### Features
- [ ] add aliases
- [ ] add aliase options (ex DOS, bash, zsh, etc);
- [ ] add user defined aliases
- [ ] add simple scripting
- [ ] add a config file


## Stable Release 1.0.0
### General
- [ ] all TODO list items completed
- [ ] software is confirmed stable
- [ ] software confirmed bug free
- [ ] software confirmed to not have memory leaks
- [ ] software is optimized for speed and size
- [ ] finish scripting


## Wish List
- [ ] be a competitior for bash/zsh/fish
- [ ] make a windows version
- [ ] POSIX compliant


# Completed
List of completed items from previous releases

## Alpha 0.0.4
### General:
- [X] ~~create a road map~~
- [X] ~~make command history be located in the home dir (ex ~/.qsh.history)~~
- [X] ~~test other libc implementations (*musl*, uClibc, etc)~~
- [X] ~~write a manpage~~

### Features:
- [X] ~~add command line args~~
- [X] ~~handle ctrl^C (return to prompt, instead of closing)~~
- [X] ~~add '~' operator to point to the users home dir (and use it for prompt too)~~
- [X] ~~use history file to populate auto complete~~
- [X] ~~use current directory to populate auto complete~~


## Alpha 0.0.3
### General:
- [X] ~~create a road map~~
- [X] ~~make command history be located in the home dir (ex ~/.qsh.history)~~
- [X] ~~test other libc implementations (*musl*, uClibc, etc)~~

### Features:
- [X] ~~handle ctrl^C (return to prompt, instead of closing)~~
- [X] ~~add '~' operator to point to the users home dir (and use it for prompt too)~~


## Alpha 0.0.2
### General:
- [X] ~~add info for GNU readline and author info on linenoise-mob in README~~
- [X] ~~be *usable* as a default shell~~
- [X] ~~document and fix known bugs~~
- [X] ~~add to github account~~
- [X] ~~pretty up the README (ex links and pictures)~~
- [X] ~~prioritize TODO list~~
- [X] ~~come up with a list of commands~~
- [X] ~~copy over old read me~~
- [X] ~~mark all other quick term projects as deprecated~~
- [X] ~~move TODO to its own file~~
- [X] ~~add a license (MIT or GPLv2)~~
- [X] ~~add color to prompt~~
- [X] ~~audit code for memory leaks~~
- [X] ~~audit code for buffer overflows~~
- [X] ~~create a make file~~
- [X] ~~understand gcc -O option ([optimization](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html))~~
- [X] ~~fix [this bug](https://github.com/antirez/linenoise/issues/158) in linenoise lib~~

### Features:
- [X] ~~basic features (ex run a command, change directories)~~
- [X] ~~add a line editing feature~~

