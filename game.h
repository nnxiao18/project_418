#ifndef GAME_H
#define GAME_H

#include <vector>

// A template that takes a State and Move type as parameters and yields an
// abstract class specifying the functions which operate on the State and Move
// types, defining a Game's behavior.
template <class State, class Move>
class Game {
  protected:
    State current_state_;
  public:
    enum GameStatus {
        kFirstPlayerTurn,
        kSecondPlayerTurn,
        kFirstPlayerWon,
        kSecondPlayerWon,
        kTie,
    };
    // Returns a list of available moves at the current state of the game.
    virtual std::vector<Move> availableMoves() const = 0;
    virtual GameStatus status() const = 0;
    // Returns the value of the current state according to a simple heuristic
    // calculation.
    // NOTE: This really belongs in the solver, not the game, since the
    // heuristic function is not inherent to the game and can be changed to
    // affect the solver's results. But it can't be implemented without
    // knowledge of the State type, so this is still cleaner.
    virtual int leafEvalState() const = 0;
    // Plays the move to update the current state of the game.
    virtual void playMove(Move move) = 0;
    // Resets the current state to the initial state.
    virtual void resetState() = 0;
};

#endif  /* GAME_H */
