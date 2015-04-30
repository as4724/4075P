CC = g++
LD = g++
CFLAGS = -c -g -O -std=c++0x -Ic:/gsl -lgsl -lgslcblas -fopenmp
#CFLAGS = -c -g -O -std=c++0x -Ic:/gsl -lgsl -lgslcblas
LDFLAGS = -Lc:/gsl
LIBS = -ldl -lgsl -lgslcblas

PROG_OBJS = main.o functions.o

TGTS = main.out

$(TGTS):$(PROG_OBJS)
	$(CC) $(LFLAGS) $(PROG_OBJS) $(LIBS) -o $(TGTS)
 
.SUFFIXES:.cpp
 
.cpp.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f $(TGTS)
	rm -f *.o
