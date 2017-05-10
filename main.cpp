#include <iostream>

#include "cycle_timer.h"
#include "tic_tac_toe_game.h"
#include "heuristic_solver.h"
#include "sequential_minimax_solver.h"
#include "sequential_alphabeta_solver.h"

#define REPEAT_TRIALS 10

int main() {
    std::cout << "----- HEURISTIC -----" << std::endl;
    double total_time = 0;
    for (int trial = 0; trial < REPEAT_TRIALS; ++trial) {
        HeuristicSolver<TicTacToeGame> hs;
        double trial_start = CycleTimer::currentSeconds();
        for (int i = 0; i < 9; ++i) {
            hs.playBestMove();
        }
        double trial_end = CycleTimer::currentSeconds();
        total_time += trial_end - trial_start;
        std::cout << "Trial " << trial << std::endl;
    }
    std::cout << "Seconds elapsed playing moves: " << total_time << std::endl;

    std::cout << "----- SEQUENTIAL MINIMAX -----" << std::endl;
    total_time = 0;
    for (int trial = 0; trial < REPEAT_TRIALS; ++trial) {
        SequentialMinimaxSolver<TicTacToeGame, 9> sms;
        double trial_start = CycleTimer::currentSeconds();
        for (int i = 0; i < 9; ++i) {
            sms.playBestMove();
        }
        double trial_end = CycleTimer::currentSeconds();
        total_time += trial_end - trial_start;
        std::cout << "Trial " << trial << std::endl;
    }
    std::cout << "Seconds elapsed playing moves: " << total_time << std::endl;

    std::cout << "----- SEQUENTIAL ALPHA-BETA -----" << std::endl;
    total_time = 0;
    for (int trial = 0; trial < REPEAT_TRIALS; ++trial) {
        SequentialAlphaBetaSolver<TicTacToeGame, 9> sabs;
        double trial_start = CycleTimer::currentSeconds();
        for (int i = 0; i < 9; ++i) {
            sabs.playBestMove();
        }
        double trial_end = CycleTimer::currentSeconds();
        total_time += trial_end - trial_start;
        std::cout << "Trial " << trial << std::endl;
    }
    std::cout << "Seconds elapsed playing moves: " << total_time << std::endl;

    return 0;
}
