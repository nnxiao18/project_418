#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "game.h"

// TODO: Change these to actual types that represent states and moves.
typedef int ChessState;
typedef int ChessMove;

class ChessGame : public Game<ChessState, ChessMove> {
  public:
    ChessGame() : Game<ChessState, ChessMove>() {
        resetState();
    }
    virtual std::vector<ChessMove> availableMoves() const;
    virtual GameStatus status() const;
    virtual int leafEvalState() const;
    virtual void playMove(ChessMove move);
    virtual void resetState();
    virtual void undoMove(ChessMove move);
};

#endif  /* CHESS_GAME_H */
