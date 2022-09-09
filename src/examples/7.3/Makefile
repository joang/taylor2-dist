
CC=gcc
CFLAGS=-c 
LFLAGS=-lm 

TAY=taylor $(RUNARG)

MODEL=perturbation.eq

HDR=taylor.h
SRC=perturbation.c main_params.c
OBJ=$(SRC:.c=.o)
OBJS=$(OBJ:.cpp=.o)
TGT=a.out

all: $(TGT)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

a.out: $(OBJS) 
	$(CC) $^ -o $@ $(LFLAGS)

taylor.h: $(MODEL) 
	$(TAY) -header -name perturbation -o $@ $(MODEL) 

perturbation.c: $(MODEL) $(HDR)
	$(TAY) -step -jet -o $@ -name perturbation $<

clean:
	-rm -f $(OBJS) $(TGT)
