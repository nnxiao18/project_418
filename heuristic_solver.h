#ifndef HEURISTIC_SOLVER_H
#define HEURISTIC_SOLVER_H

template <class Game>
class HeuristicSolver : public GameSolver<Game>{
  using GameSolver<Game>::game_;
  public:
    virtual int bestNextMove();
};

template <class Game>
int HeuristicSolver<Game>::bestNextMove() {
    return game_.leafEvalState();
}

#endif  /* HEURISTIC_SOLVER_H */
