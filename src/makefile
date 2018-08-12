qsh: ui.c ui.h libs/linenoise/linenoise.c libs/linenoise/linenoise.h commands/help.h commands/c.h
	$(CC) -Wall -Ofast -o qsh libs/linenoise/linenoise.c ui.c
gnu: ui.c ui.h commands/help.h commands/c.h
	$(CC) -Wall -Ofast -DGNU -lreadline -o qsh.gnu ui.c
tiny: ui.c ui.h commands/help.h commands/c.h
	$(CC) -Wall -Ofast -DTINY -o qsh.tiny ui.c
static: ui.c ui.h libs/linenoise/linenoise.c libs/linenoise/linenoise.h commands/help.h commands/c.h
	$(CC) -Wall -Ofast -o qsh.static libs/linenoise/linenoise.c ui.c -static

clean:
	rm qsh*
	rm history.log

debug:	ui.c ui.h libs/linenoise/linenoise.c libs/linenoise/linenoise.h commands/help.h commands/c.h
	# Used if you want debugging features on
	$(CC) -Wall -Wextra -Og -g -DDEBUGGING -o qsh.debugging libs/linenoise/linenoise.c ui.c

