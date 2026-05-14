CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = cdr_engine

all: $(TARGET)

$(TARGET): main.o cdr.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o cdr.o

main.o: main.c cdr.h
	$(CC) $(CFLAGS) -c main.c

cdr.o: cdr.c cdr.h
	$(CC) $(CFLAGS) -c cdr.c

clean:
	rm -f *.o $(TARGET)
