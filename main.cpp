#include <iostream>

#include "cycle_timer.h"
#include "tic_tac_toe_game.h"
#include "sequential_minimax_solver.h"

#define REPEAT_TRIALS 10

int main() {
    double total_time = 0;
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
    return 0;
}
