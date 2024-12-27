TARGETS=final

all:			${TARGETS}

final:		main.c action.c evidence.c house.c room.c init.c logger.c utils.c
				gcc -Wall -Wextra -Werror -g -o final main.c action.c evidence.c house.c room.c init.c logger.c utils.c

clean:
				rm -f ${TARGETS}