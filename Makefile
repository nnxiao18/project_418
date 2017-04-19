CC = g++
CFLAGS = -c -Wall -Wextra -O2 -std=c++11
LFLAGS = -Wall
DEBUG = -g
OBJS = chess_game.o main.o

main: $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

chess_game.o: chess_game.h chess_game.cpp
	$(CC) chess_game.cpp $(CFLAGS)

main.o: main.cpp game_solver.h
	$(CC) main.cpp $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o main
