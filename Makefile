CC=g++
CFLAGS=-Wall -Wextra -Wpedantic
EXECUTABLE=calendar

all:
	$(CC) $(CFLAGS) src/main.cpp -o $(EXECUTABLE)

clean:
	rm -rf *.o $(EXECUTABLE)