CC=g++
CFLAGS=-c -std=c++11
LFLAGS=
OBJS=main.o Memory.o M6502.o 
#M6522.o

%.o: %.cxx %.h
	$(CC) $(CFLAGS) -o $@ $<
	
%.o: %.cxx
	$(CC) $(CFLAGS) -o $@ $<

all: pc1541

pc1541: $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS)

clean:
	rm -rf *.o
	rm -rf pc1541

run: pc1541
	./$<

debug: pc1541
	valgrind --leak-check=full --track-origins=yes ./$<
