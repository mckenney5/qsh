QSH_DEPENDS = src/ui.c src/ui.h src/commands/help.h src/commands/c.h src/libs/linux.h src/libs/safestring.h
LINENOISE_DEPENDS = src/libs/linenoise/linenoise.c src/libs/linenoise/linenoise.h
C_FLAGS = -Wall -Wformat-truncation=0 -Ofast

qsh.out: $(QSH_DEPENDS) $(LINENOISE_DEPENDS)
	$(CC) $(C_FLAGS) -o qsh.out src/libs/linenoise/linenoise.c src/ui.c
gnu: $(QSH_DEPENDS)
	$(CC) $(C_FLAGS) -DGNU -lreadline -o qsh.gnu.out src/ui.c
tiny: $(QSH_DEPENDS)
	$(CC) $(C_FLAGS) -DTINY -o qsh.tiny.out src/ui.c
static: $(QSH_DEPENDS) $(LINENOISE_DEPENDS) 
	$(CC) $(C_FLAGS) -Os -o qsh.static.out src/libs/linenoise/linenoise.c src/ui.c -static
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
	$(CC) -Wall -Wextra -O0 -g -DDEBUGGING -o qsh.debugging.out src/libs/linenoise/linenoise.c src/ui.c

