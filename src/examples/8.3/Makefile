 
CC=gcc
CFLAGS=-c
LFLAGS=-lm

TAY=../../taylor  $(RUNARG)

MODEL=vf.eq

HDR=vf.h
SRC=vf.c io_myjet.c
OBJ=$(SRC:.c=.o)
OBJS=$(OBJ:.cpp=.o)
TGT=a.out

all: $(TGT)

.c.o:
	$(CC) $(CFLAGS) $< -o $@
%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@


vf.h: $(MODEL)
	$(TAY) -o $@ -header $<

vf.c: vf.h $(MODEL) 
	$(TAY) -o $@ -jet -jet_helper -step -headername $^

a.out: $(OBJS) 
	$(CC) $^ -o $@ $(LFLAGS)

clean:
	-rm -f $(OBJS) $(TGT) vf.c vf.h kk.txt
