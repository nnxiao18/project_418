#ifndef GAME_SOLVER_H
#define GAME_SOLVER_H

// A template that takes a Game type and yields an abstract class specifying
// the function which operates on the Game to find the best next move.
// TODO: Game should be derived from a class created from template Game<State,
// Move> in game.h. Don't know how to specify this in a non-hacky way, though
// this seems to be what we want:
// http://stackoverflow.com/questions/17574231/using-a-static-assert-to-determine-if-a-specific-template-parameter-is-a-specifi
// For now, you could try to apply this template to a class not derived from an
// application of Game<State, Move>, but it will just not compile since the
// class will not have the functions it tries to call.
template <class Game>
class GameSolver {
  protected:
    Game game_;
  public:
    // Default constructor: initialize game_ at its initial state.
    GameSolver() = default;
    // Determines and plays the best move for the current state.
    virtual void playBestMove() = 0;
    // Reset game_ using its resetState function.
    void resetState() {
        game_.resetState();
    }
};

#endif  /* GAME_SOLVER_H */
