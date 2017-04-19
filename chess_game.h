#ifndef CHESS_GAME_H
#define CHESS_GAME_H

class ChessGame {
    typedef int State;
    typedef int Move;
  private:
    State current_state_;
  public:
    // Default constructor: construct a game at the initial state.
    ChessGame();
    // TODO: Change to vector<Move>.
    // Returns a list of available moves at the current state of the game.
    Move availableMoves();
    // TODO: Switch to return enum (-1, 0, 1)
    // TODO: Explain enum
    bool isOver();
    // Plays the move to update the current state of the game.
    void playMove(Move move);
    // Resets the current state to the initial state for chess.
    void resetState();
};

#endif  /* CHESS_GAME_H */
