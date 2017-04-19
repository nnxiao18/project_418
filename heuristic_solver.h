#ifndef HEURISTIC_SOLVER_H
#define HEURISTIC_SOLVER_H

#include "game_solver.h"

// TODO: An incomplete example of how to extend the GameSolver<Game> template
// to create a Solver<Game> template with a specific implementation strategy.
template <class Game>
class HeuristicSolver : public GameSolver<Game>{
  using GameSolver<Game>::game_;
  public:
    virtual void playBestMove();
};

template <class Game>
void HeuristicSolver<Game>::playBestMove() {
    return;
}

#endif  /* HEURISTIC_SOLVER_H */
