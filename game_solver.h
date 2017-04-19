#ifndef GAME_SOLVER_H
#define GAME_SOLVER_H

template <class Game>
class GameSolver {
  protected:
    Game game_;
  public:
    // Default constructor: initialize game_ at its initial state.
    GameSolver() = default;
    // Returns the index of the best next move to play.
    virtual int bestNextMove() = 0;
};

#endif  /* GAME_SOLVER_H */
