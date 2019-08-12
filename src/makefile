qsh: ui.c ui.h libs/linenoise/linenoise.c libs/linenoise/linenoise.h commands/help.h commands/c.h libs/linux.h libs/safestring.h
	$(CC) -Wall -Wformat-truncation=0 -Ofast -o qsh libs/linenoise/linenoise.c ui.c
gnu: ui.c ui.h commands/help.h commands/c.h libs/linux.h libs/safestring.h
	$(CC) -Wall -Wformat-truncation=0 -Ofast -DGNU -lreadline -o qsh.gnu ui.c
tiny: ui.c ui.h commands/help.h commands/c.h libs/linux.h libs/safestring.h
	$(CC) -Wall -Wformat-truncation=0 -Ofast -DTINY -o qsh.tiny ui.c
static: ui.c ui.h libs/linenoise/linenoise.c libs/linenoise/linenoise.h commands/help.h commands/c.h libs/linux.h libs/safestring.h
	$(CC) -Wall -Wformat-truncation=0 -Os -o qsh.static libs/linenoise/linenoise.c ui.c -static

clean:
	rm qsh*
	rm ~/.qsh.history

debug:	ui.c ui.h libs/linenoise/linenoise.c libs/linenoise/linenoise.h commands/help.h commands/c.h libs/linux.h libs/safestring.h
	# Used if you want debugging features on
	$(CC) -Wall -Wextra -O0 -g -DDEBUGGING -o qsh.debugging libs/linenoise/linenoise.c ui.c

