#include <iostream>

#include "cycle_timer.h"
#include "tic_tac_toe_game.h"
#include "heuristic_solver.h"
#include "sequential_minimax_solver.h"
#include "sequential_nocopy_minimax_solver.h"
#include "sequential_alphabeta_solver.h"
#include "sequential_nocopy_alphabeta_solver.h"
#include "omp_minimax_contention_solver.h"
#include "omp_minimax_local_solver.h"

#define REPEAT_TRIALS 10
#define PLIES 9

void runTrials(GameSolver<TicTacToeGame>& solver, const char* name) {
    std::cout << "----- " << name << " -----" << std::endl;
    double total_time = 0;
    std::cout << "Completed trials: ";
    for (int trial = 0; trial < REPEAT_TRIALS; ++trial) {
        solver.resetState();
        double trial_start = CycleTimer::currentSeconds();
        for (int i = 0; i < 9; ++i) {
            solver.playBestMove();
        }
        double trial_end = CycleTimer::currentSeconds();
        total_time += trial_end - trial_start;
        if (trial > 0) {
            std::cout << ", ";
        }
        std::cout << trial;
    }
    std::cout << std::endl;
    std::cout << "Seconds elapsed playing moves: " << total_time << std::endl;
}

int main() {
    HeuristicSolver<TicTacToeGame> hs;
    runTrials(hs, "HEURISTIC");

    SequentialMinimaxSolver<TicTacToeGame, PLIES> sms;
    runTrials(sms, "SEQUENTIAL MINIMAX");

    SequentialNoCopyMinimaxSolver<TicTacToeGame, PLIES> sncms;
    runTrials(sncms, "SEQUENTIAL (NO COPY) MINIMAX");

    SequentialAlphaBetaSolver<TicTacToeGame, PLIES> sabs;
    runTrials(sabs, "SEQUENTIAL ALPHA-BETA");

    SequentialNoCopyAlphaBetaSolver<TicTacToeGame, PLIES> sncabs;
    runTrials(sncabs, "SEQUENTIAL (NO COPY) ALPHA-BETA");

    OmpMinimaxContentionSolver<TicTacToeGame, PLIES> omcs;
    runTrials(omcs, "OPENMP (W/ CONTENTION) MINIMAX");

    OmpMinimaxLocalSolver<TicTacToeGame, PLIES> omls;
    runTrials(omls, "OPENMP (THREAD-LOCAL) MINIMAX");

    return 0;
}
