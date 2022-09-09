 
CC=gcc
CFLAGS=-c
LFLAGS=-lm

TAY=taylor  $(RUNARG)

MODEL=lorenz5.eq

HDR=lorenz5.h
SRC=lorenz5.c main_equilibria.c
OBJ=$(SRC:.c=.o)
OBJS=$(OBJ:.cpp=.o)
TGT=a.out

all: $(TGT)

.c.o:
	$(CC) $(CFLAGS) $< -o $@
%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@


lorenz5.h: $(MODEL)
	$(TAY) -o $@ -header $<

lorenz5.c: lorenz5.h $(MODEL) 
	$(TAY) -o $@ -headername $^ -jet -jet_helper -step -o $@ $(MODEL) 

a.out: $(OBJS) 
	$(CC) $^ -o $@ $(LFLAGS)

clean:
	-rm -f $(OBJS) $(TGT) lorenz5.c lorenz5.h
