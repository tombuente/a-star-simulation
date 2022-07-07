NAME = astar
CC = gcc
CFLAGS = -Wall -MMD
LDFLAGS = -lncurses -lm


SRCS = \
	${SRCDIR}/main.c \
	${SRCDIR}/a_star.c \
	${SRCDIR}/linked_list.c \
	${SRCDIR}/map_reader.c \

SRCDIR = ./src

OBJS = ${SRCS:.c=.o}
DEP = $(OBJS:%.o=%.d)

RM = rm -f

%.o: %.c
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LDFLAGS} -o ${NAME}

all: ${NAME}

.PHONY: clean
clean:
	${RM} ${OBJS} ${DEP} ${NAME}

-include $(DEP)
