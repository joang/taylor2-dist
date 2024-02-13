 
CC=gcc
CFLAGS=-c
LFLAGS=-lm

TAY=../../taylor  $(RUNARG)

MODEL=vdp.eq

HDR=vdp.h
SRC=vdp.c main.c
OBJ=$(SRC:.c=.o)
OBJS=$(OBJ:.cpp=.o)
TGT=a.out

all: $(TGT)

.c.o:
	$(CC) $(CFLAGS) $< -o $@
%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@


vdp.h: $(MODEL)
	$(TAY) -o $@ -header $<

vdp.c: vdp.h $(MODEL) 
	$(TAY) -o $@ -jet -step -headername $^

a.out: $(OBJS) 
	$(CC) $^ -o $@ $(LFLAGS)

clean:
	-rm -f $(OBJS) $(TGT) vdp.c vdp.h
