#include <cstdio>

#include "chess_game.h"
#include "heuristic_solver.h"

int main() {
    ChessGame cg;
    ChessGame::GameOverStatus s = cg.isOver();
    printf("Status is %d\n", s);
    HeuristicSolver<ChessGame> hs;
    int v = hs.bestNextMove();
    printf("Value is %d\n", v);
    return 0;
}
