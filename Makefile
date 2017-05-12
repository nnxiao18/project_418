CC = g++
CFLAGS = -c -Wall -Wextra -O3 -std=c++11 -fopenmp
LFLAGS = -Wall -fopenmp -lpthread
DEBUG = -g
OBJS = gomoku_game.o main.o
TESTS = tic_tac_toe_game.o test_tic_tac_toe.o

main: $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

test: $(TESTS)
	$(CC) -o $@ $^ $(LFLAGS)

gomoku_game.o: gomoku_game.h gomoku_game.cpp game.h
	$(CC) gomoku_game.cpp $(CFLAGS)

tic_tac_toe_game.o: tic_tac_toe_game.h tic_tac_toe_game.cpp game.h
	$(CC) tic_tac_toe_game.cpp $(CFLAGS)

chess_game.o: chess_game.h chess_game.cpp game.h
	$(CC) chess_game.cpp $(CFLAGS)

test_tic_tac_toe.o: test_tic_tac_toe.cpp tic_tac_toe_game.h
	$(CC) test_tic_tac_toe.cpp $(CFLAGS)

main.o: main.cpp cycle_timer.h gomoku_game.h heuristic_solver.h \
		sequential_minimax_solver.h sequential_nocopy_minimax_solver.h \
		sequential_alphabeta_solver.h sequential_nocopy_alphabeta_solver.h \
		omp_contention_minimax_solver.h omp_local_minimax_solver.h \
		omp_contention_alphabeta_solver.h omp_local_alphabeta_solver.h \
		pvs_contention_alphabeta_solver.h pvs_local_alphabeta_solver.h \
		pvs_rand_contention_alphabeta_solver.h pvs_rand_local_alphabeta_solver.h \
		pvs_heur_contention_alphabeta_solver.h
	$(CC) main.cpp $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o main test
