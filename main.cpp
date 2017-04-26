#include <iostream>

#include "cycle_timer.h"
#include "tic_tac_toe_game.h"
#include "heuristic_solver.h"

int main() {
    HeuristicSolver<TicTacToeGame> hs;
    double start = CycleTimer::currentSeconds();
    for (int i = 0; i < 9; ++i) {
        hs.playBestMove();
        std::cout << "Played move " << i << std::endl;
    }
    double end = CycleTimer::currentSeconds();
    std::cout << "Seconds elapsed playing moves: " << (end - start) <<
            std::endl;
    return 0;
}
