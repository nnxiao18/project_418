#include "chess_game.h"

// TODO: All functions here are just stubs that typecheck. Actual
// implementation for chess is needed.

// TODO: Move generation will be the hardest part by far. We can find existing
// algorithms and re-implement them, but even that might be quite hard... we
// should maybe consider starting with an easier game than chess.
std::vector<ChessMove> ChessGame::availableMoves() const {
    return std::vector<ChessMove>();
}

ChessGame::GameStatus ChessGame::status() const {
    return kFirstPlayerTurn;
}

int ChessGame::leafEvalState() const {
    return 2;
}

void ChessGame::playMove(ChessMove move) {
    move += 5;
    return;
}

void ChessGame::resetState() {
    current_state_ = 0;
    return;
}

void ChessGame::undoMove(ChessMove move) {
    move += 5;
    return;
}
