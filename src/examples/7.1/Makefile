
CC=gcc
CFLAGS=-c 
LFLAGS=-lm 

TAY=taylor $(RUNARG)

MODEL=lorenz1.eq

SRC=lorenz.c
OBJ=$(SRC:.c=.o)
OBJS=$(OBJ:.cpp=.o)
TGT=a.out

all: $(TGT)

.c.o:
	$(CC) $(CFLAGS) $< -o $@
%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

a.out: $(OBJS) 
	$(CC) $^ -o $@ $(LFLAGS)

lorenz.c: $(MODEL) 
	$(TAY) -o lorenz.c $(MODEL) 

clean:
	-rm -f $(OBJS) $(TGT) lorenz.c
