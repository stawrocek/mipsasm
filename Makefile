CC=g++
CFLAGS=-Wall -std=c++17 -g 

default: mipsasm

mipsasm: main.o Line.o MipsParser.o Section.o
	$(CC) $(CFLAGS) main.o Line.o MipsParser.o Section.o -o mipsasm

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -c

Line.o: Line.cpp
	$(CC) $(CFLAGS) Line.cpp -c

MipsParser.o: Line.cpp MipsParser.cpp
	$(CC) $(CFLAGS) MipsParser.cpp -c

Section.o: Section.cpp
	$(CC) $(CFLAGS) Section.cpp -c


clean:
	rm -f *.o mipsasm
