#include <cstdio>

#include "chess_game.h"
#include "heuristic_solver.h"

int main() {
    ChessGame cg;
    ChessGame::GameStatus s = cg.status();
    printf("Status is %d\n", s);
    HeuristicSolver<ChessGame> hs;
    hs.playBestMove();
    printf("Played move\n");
    return 0;
}
