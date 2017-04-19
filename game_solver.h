#ifndef GAME_SOLVER_H
#define GAME_SOLVER_H

template <class Game>
class GameSolver {
  private:
    Game game_;
  public:
    // Default constructor: initialize game_ at its initial state.
    GameSolver() = default;
    // Returns the index of the best next move to play.
    int bestNextMove();
};

template <class Game>
int GameSolver<Game>::bestNextMove() {
    return game_.leafEvalState();
}

#endif  /* GAME_SOLVER_H */
