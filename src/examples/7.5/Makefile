 
CC=gcc
CFLAGS=-c 
LFLAGS=-lm 

TAY=taylor  $(RUNARG)

MODEL=lorenz3.eq

HDR=lorenz3.h
SRC=lorenz3.c main_varieq.c
OBJ=$(SRC:.c=.o)
OBJS=$(OBJ:.cpp=.o)
TGT=a.out

all: $(TGT)

.c.o:
	$(CC) $(CFLAGS) $< -o $@
%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@


lorenz3.h: $(MODEL)
	$(TAY) -o $@ -header $<


lorenz3.c: $(MODEL) 
	$(TAY) -o lorenz3.c -header_name lorenz3.h -jet -jet_helper -step -o $@ $(MODEL) 

a.out: $(OBJS) 
	$(CC) $^ -o $@ $(LFLAGS)

clean:
	-rm -f $(OBJS) $(TGT) lorenz3.c lorenz3.h
