#include <iostream>

#include "tic_tac_toe_game.h"
#include "heuristic_solver.h"

int main() {
    HeuristicSolver<TicTacToeGame> hs;
    for (int i = 0; i < 9; ++i) {
        hs.playBestMove();
        std::cout << "Played move " << i << std::endl;
    }
    return 0;
}
