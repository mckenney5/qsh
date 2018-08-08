qsh: ui.c ui.h libs/linenoise.c libs/linenoise.h commands/help.h commands/c.h
	$(CC) -Wall -Ofast -o qsh libs/linenoise.c ui.c
GNU: ui.c ui.h commands/help.h commands/c.h
	$(CC) -Wall -Ofast -DGNU -lreadline -o qsh.gnu ui.c
TINY: ui.c ui.h commands/help.h commands/c.h
	$(CC) -Wall -Ofast -DTINY -o qsh.tiny ui.c
STATIC: ui.c ui.h libs/linenoise.c commands/help.h commands/c.h
	$(CC) -Wall -Ofast -o qsh.static libs/linenoise.c ui.c -static

clean:
	rm qsh*
	rm history.log

DEBUG:	ui.c ui.h libs/linenoise.c libs/linenoise.h commands/help.h commands/c.h
	# Used if you want debugging features on
	$(CC) -Wall -Wextra -Og -g -DDEBUGGING -o qsh.debugging libs/linenoise.c ui.c

