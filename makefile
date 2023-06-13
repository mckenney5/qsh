#TODO FIXME
CPP_FILES = src/ui.cpp src/interpreter.cpp src/libs/easy_token.cpp src/libs/linenoise/linenoise.c
H_FILES = src/ui.h src/interpreter.h src/libs/easy_token.h src/libs/linenoise/linenoise.h
QSH_DEPENDS = $(CPP_FILES) $(H_FILES)
LINENOISE_DEPENDS = src/libs/linenoise/linenoise.c src/libs/linenoise/linenoise.h
C_FLAGS = -Wall -Ofast
CPP = g++

qsh.out: $(QSH_DEPENDS) $(LINENOISE_DEPENDS)
	$(CPP) $(C_FLAGS) $(CPP_FILES)
	mv a.out qsh.out
static: $(QSH_DEPENDS) $(LINENOISE_DEPENDS) 
	$(CPP) $(C_FLAGS) -Os -o qsh.static.out $(CPP_FILES) -static
qsh.1.gz: qsh.1
	gzip -k qsh.1

clean:
	rm -i qsh*.out
	rm -i ~/.qsh.history

install: qsh.1.gz qsh.out
	# Install the man page
	sudo mv qsh.1.gz /usr/share/man/man1/
	# Install the program
	sudo cp qsh.out /usr/bin/qsh

uninstall:
	sudo rm -i /usr/share/man/man1/qsh.1.gz
	sudo rm -i /usr/bin/qsh

debug: $(QSH_DEPENDS) $(LINENOISE_DEPENDS)
	# Used if you want debugging features on
	$(CPP) -Wall -Wextra -O0 -g -DDEBUGGING -o qsh.debugging.out $(CPP_FILES)

