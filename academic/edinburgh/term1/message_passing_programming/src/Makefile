MF=	Makefile

#COMPILER
CC=	mpicc

CFLAGS=	-fastsse
LFLAGS=	-lm

EXE=	execute

SRC= \
	main.c\
	pgmio.c\
        functions.c

INC = \
	pgmio.h\
        functions.h

#
# No need to edit below this line
#

.SUFFIXES:
.SUFFIXES: .c .o

OBJ=	$(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -c $<

all:	$(EXE)

$(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFLAGS)

$(OBJ):	$(MF) $(INC)

clean:
	rm -f $(OBJ) $(EXE) core processedimage.pgm *~ *.o execute script.sge.*
