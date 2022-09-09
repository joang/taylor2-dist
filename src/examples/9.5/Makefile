
CC=gcc
CFLAGS=-c 
LFLAGS=-lm 

TAY=taylor $(RUNARG)

MODEL=lorenz1.eq

HDR=taylor.h
SRC1=lorenz.c main1.c
SRC2=lorenz.c main2.c
OBJ1=$(SRC1:.c=.o)
OBJ2=$(SRC2:.c=.o)
TGT=ex1 ex2

all: $(TGT)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

ex1: $(OBJ1) 
	$(CC) $^ -o $@ $(LFLAGS)

ex2: $(OBJ2) 
	$(CC) $^ -o $@ $(LFLAGS)

lorenz.c: taylor.h $(MODEL) 
	$(TAY) -name lorenz -o $@ -jet -step -headername $^

taylor.h: $(MODEL)
	$(TAY) -name lorenz -o $@ -header $<

clean:
	-rm -f $(OBJ1) $(OBJ2) $(TGT) lorenz.c taylor.h
