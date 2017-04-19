#include <cstdio>

#include "chess_game.h"
#include "heuristic_solver.h"

int main() {
    ChessGame cg;
    ChessGame::GameOverStatus s = cg.isOver();
    printf("Status is %d\n", s);
    HeuristicSolver<ChessGame> hs;
    hs.playBestMove();
    printf("Played move\n");
    return 0;
}
