 
CC=gcc
CFLAGS=-c  -fopenmp
LFLAGS=-lm  -fopenmp

TAY=taylor  $(RUNARG)

MODEL=lorenz4.eq

HDR=lorenz4.h
SRC=lorenz4.c main_omp.c
OBJ=$(SRC:.c=.o)
OBJS=$(OBJ:.cpp=.o)
TGT=a.out

all: $(TGT)

.c.o:
	$(CC) $(CFLAGS) $< -o $@
%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@


lorenz4.h: $(MODEL)
	$(TAY) -o $@ -header $<

lorenz4.c: lorenz4.h $(MODEL) 
	$(TAY) -o $@ -headername $^ -jet -jet_helper -step -o $@ $(MODEL) 
	cat lorenz4.c | sed -e '/extern MY_FLOAT SS;/a #pragma omp threadprivate(RR)' > x
	mv x lorenz4.c

a.out: $(OBJS) 
	$(CC) $^ -o $@ $(LFLAGS)

clean:
	-rm -f $(OBJS) $(TGT) lorenz4.c lorenz4.h
