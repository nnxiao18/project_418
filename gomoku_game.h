#ifndef GOMOKU_GAME_H
#define GOMOKU_GAME_H

#include <array>

#include "game.h"

#define BOARD_SIDE 15
#define BOARD_LEN (BOARD_SIDE * BOARD_SIDE)
#define K_IN_ROW 5
// Represents the state of a single square of the board.
enum SquareStatus {
    kEmpty,
    kX,
    kO,
};
// The state of the game is represented by an array of BOARD_LEN squares'
// statuses and a boolean indicating whether it is currently the first player's
// turn.
typedef std::pair<std::array<SquareStatus, BOARD_LEN>, bool> GomokuState;
// A move is represented by its index in the board. (It's not necessary to
// represent whether it is X or O, because that is determined by the current
// state of the board).
typedef int GomokuMove;

class GomokuGame : public Game<GomokuState, GomokuMove> {
  public:
    GomokuGame() : Game<GomokuState, GomokuMove>() {
        resetState();
    }
    virtual std::vector<GomokuMove> availableMoves() const;
    virtual GameStatus status() const;
    // More positive return value is better for first player, and more negative
    // is better for second player. The algorithm is: for every occupied
    // square, look K_IN_ROW up, left, right, down, and the diagonals. For
    // each lane, if it is occupied by the opposite player's piece, it's worth
    // zero points. If not, it's worth 1 + the number of other pieces in it for
    // the current player.
    virtual int leafEvalState() const;
    // NOTE: Does not validate the given move; calling with an invalid move for
    // the current state can get you into an invalid game state.
    virtual void playMove(GomokuMove move);
    virtual void resetState();
    // Just makes the specified square kEmpty.
    virtual void undoMove(GomokuMove move);
    // Get a text representation of the game state, for testing and debugging.
    std::string stateString() const;
};

#endif  /* GOMOKU_GAME_H */
