RUNARG=-jlib jet1_1

CC=gcc
CFLAGS=-c 
LFLAGS=-lm 

TAY=taylor  $(RUNARG)

MODEL=lorenz2.eq

HDR=lorenz2.h
SRC=lorenz2.c main_lyap.c
OBJ=$(SRC:.c=.o)
OBJS=$(OBJ:.cpp=.o)
TGT=a.out

all: $(TGT)

.c.o:
	$(CC) $(CFLAGS) $< -o $@
%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@


lorenz2.h: $(MODEL)
	$(TAY) -o $@ -header $<


lorenz2.c: lorenz2.h $(MODEL) 
	$(TAY) -o lorenz2.c -jet -jet_helper -step -o $@ -header_name $^

a.out: $(OBJS) 
	$(CC) $^ -o $@ $(LFLAGS)

clean:
	-rm -f $(OBJS) $(TGT) lorenz2.c lorenz2.h
