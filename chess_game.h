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
    // Returns the value of the current state according to a simple heuristic
    // calculation.
    // NOTE: This really belongs in the solver, not the game, since the
    // heuristic function is not inherent to the game and can be changed to
    // affect the solver's results. But it can't be implemented without
    // knowledge of the State type, so this is still cleaner.
    int leafEvalState();
    // Plays the move to update the current state of the game.
    void playMove(Move move);
    // Resets the current state to the initial state for chess.
    void resetState();
};

#endif  /* CHESS_GAME_H */
