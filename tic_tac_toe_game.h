#ifndef TIC_TAC_TOE_GAME_H
#define TIC_TAC_TOE_GAME_H

#include <array>

#include "game.h"

// Standard tic tac toe board size.
#define BOARD_SIDE 3
#define BOARD_LEN 9
// Represents the state of a single square of the board.
enum SquareStatus {
    kEmpty,
    kX,
    kO,
};
// The state of the game is represented by an array of 9 squares' states and a
// boolean indicating whether it is currently the first player's turn.
typedef std::pair<std::array<SquareStatus, BOARD_LEN>, bool> TicTacToeState;
// A move is represented by its index in the board. (It's not necessary to
// represent whether it is X or O, because that is determined by the current
// state of the board).
typedef int TicTacToeMove;

class TicTacToeGame : public Game<TicTacToeState, TicTacToeMove> {
  public:
    TicTacToeGame() : Game<TicTacToeState, TicTacToeMove>() {
        resetState();
    }
    virtual std::vector<TicTacToeMove> availableMoves();
    virtual GameOverStatus isOver();
    virtual int leafEvalState();
    // NOTE: Does not validate the given move; calling with an invalid move for
    // the current state can get you into an invalid game state.
    virtual void playMove(TicTacToeMove move);
    virtual void resetState();
    // Get a text representation of the game state, for testing and debugging.
    std::string stateString();
};

#endif  /* TIC_TAC_TOE_GAME_H */
