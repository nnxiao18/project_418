CC=g++
CFLAGS=-Wall -Wextra -O2 -std=c++11 -I.
DEPS = game.h
OBJ = game.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $^ $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o main
