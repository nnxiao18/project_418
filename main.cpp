#include <stdio.h>

#include "chess_game.h"
#include "game_solver.h"

int main() {
    GameSolver<ChessGame> gs;
    int v = gs.bestNextMove();
    printf("Value is %d\n", v);
    return 0;
}
