Quick Shell
==============
qsh — _A small, lightweight, command interpreter_

![screen shot](https://github.com/mckenney5/qsh/raw/master/sc.png)

## Description
This is the rewrite of quick terminal, quick terminal server edition, and QTerminal. 
Quick Shell aims at replacing `bash` or `zsh` with a smaller and lighter program and follows
the [suckless philosophy](https://suckless.org/philosophy/). (note this project is not
associated with suckless).

## Project Meta
This software is in a **working alpha** state.
The software may not work and may have drastic changes in the future.

## License
This code (see exception) is licensed under the MIT license. See 'LICENSE' for details. 

Note, the source code for [linenoise-mob](https://github.com/rain-1/linenoise-mob) is also 
included in this repo which is under the BSD 2-Clause "Simplified" License. 
See [src/libs/linenoise/LICENSE](https://raw.githubusercontent.com/mckenney5/qsh/master/src/libs/linenoise/LICENSE) 
for more details.

## Compiling and Testing
This program allows a couple of compiling options.

The simplest way is to compile by `make`

##### Make Options:
* `make gnu`		: makes the user interface use [GNU readline](https://tiswww.case.edu/php/chet/readline/rltop.html)
* `make tiny`		: makes the UI only use fgets (aka no line editing, like sh(1))
* `make debug`		: adds debugging symbols to the program, along with verbose output
* `make static`		: statically links program (note: this should be done only with musl clib)
* `make`		: makes the UI use the [linenoise-mob library](https://github.com/rain-1/linenoise-mob)
* `make install`	: moves qsh to the /usr/bin file and installs the manpage
* `make uninstall`	: deletes /usr/bin/qsh and the man file from the system

##### (optional) Colors:
* NO_COLOR (`-DNO_COLOR`)	: disables color in the prompt **ONLY**

## How to contribute
Take a look at the ROADMAP for what still needs to get done for the next version.

Then:
1. Fork the repository
2. Compile
3. Test
4. Make changes
5. **Test again**
6. After all of that **_then_** submit a merge/pull request

