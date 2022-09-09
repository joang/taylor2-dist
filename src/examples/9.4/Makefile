
CC=gcc
CFLAGS=-c 
LFLAGS=-lm 

TAY=taylor $(RUNARG)

MODEL=model.eq

HDR=jdata.h
SRC=jdata.c jdata_main.c
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

jdata.c: jdata.h $(MODEL) 
	$(TAY) -name jdata -o $@ -jet -jet_helper -step -headername $^
jdata.h: $(MODEL)
	$(TAY) -name jdata -o $@ -header $<

clean:
	-rm -f $(OBJS) $(TGT) jdata.c jdata.h
