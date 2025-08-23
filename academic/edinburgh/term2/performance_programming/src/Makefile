SRC=MD.c control.c util.c 
OBJ=$(SRC:.c=.o)
CC=pgcc -tp=bulldozer 
CFLAGS= -Minfo -fastsse -Mipa=fast,inline -Munroll -Mvect -O4 -Mprefetch 

all: MD

MD: $(OBJ)
	$(CC) $(CFLAGS)  -o $@  $(OBJ) -lm


output.dat: MD input.dat
	./MD


clean:
	rm -f MD $(OBJ) bench_c.sge.* output.dat* *~

$(OBJ) : coord.h Makefile

