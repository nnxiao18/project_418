#include "chess_game.h"

ChessGame::ChessGame()
    : current_state_(2)
{
    resetState();
}

ChessGame::Move ChessGame::availableMoves() {
    return 1;
}

bool ChessGame::isOver() {
    return false;
}

int ChessGame::leafEvalState() {
    return 42;
}

void ChessGame::playMove(Move move) {
    move += 5;
    return;
}

void ChessGame::resetState() {
    current_state_ = 0;
    return;
}
