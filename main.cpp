#include <stdio.h>

#include "chess_game.h"
#include "game_solver.h"
#include "heuristic_solver.h"

int main() {
    HeuristicSolver<ChessGame> hs;
    int v = hs.bestNextMove();
    printf("Value is %d\n", v);
    return 0;
}
