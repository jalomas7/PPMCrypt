CC = g++
CFLAGS = -g -Wall
BIN = crypto

all: $(BIN)

$(BIN): $(BIN).o support.o rnumgen.o
	 $(CC) $(CFLAGS) -o $@ $(BIN).o support.o rnumgen.o

$(BIN).o: $(BIN).cpp support.h
	$(CC) $(CFLAGS) -c $(BIN).cpp

support.o: support.cpp support.h
	$(CC) $(CFLAGS) -c support.cpp

rnumgen.o: rnumgen.cpp rnumgen.h
	$(CC) $(CFLAGS) -c rnumgen.cpp
clean:
	@rm -f *.o *_wmsg.ppm $(BIN)
