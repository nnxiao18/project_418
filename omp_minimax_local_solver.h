#ifndef OMP_MINIMAX_LOCAL_SOLVER_H
#define OMP_MINIMAX_LOCAL_SOLVER_H

#include "game_solver.h"

#include <climits>
#include <omp.h>

// Computes minimax to specified depth in parallel (at top level) with OpenMP.
// Improves upon the OmpMinimaxContentionSolver by having each thread keep
// track of best_score, best_move as thread-local variables, then combining at
// end to eliminate cache contention.
template <class Game, int depth>
class OmpMinimaxLocalSolver : public GameSolver<Game> {
  static_assert(depth >= 1, "Depth must be >= 1");
  using GameSolver<Game>::game_;
  private:
    // Finds the best move for game with minimax evaluation up to plies, plays
    // it, and returns its minimax value.
    int playBestMoveForGame(Game& game, bool first_player_turn, int plies);
  public:
    virtual void playBestMove();
};

template <class Game, int depth>
void OmpMinimaxLocalSolver<Game, depth>::playBestMove() {
    switch (game_.status()) {
        case Game::kFirstPlayerWon:
        case Game::kSecondPlayerWon:
        case Game::kTie:
            // If game is over, return without action.
            return;
        case Game::kFirstPlayerTurn:
            playBestMoveForGame(game_, true, depth - 1);
            break;
        case Game::kSecondPlayerTurn:
            playBestMoveForGame(game_, false, depth - 1);
            break;
    }
}

template <class Game, int depth>
int OmpMinimaxLocalSolver<Game, depth>::playBestMoveForGame(
        Game& game, bool first_player_turn, int plies) {
    const auto moves = game.availableMoves();
    int global_best_score = first_player_turn ? INT_MIN : INT_MAX;
    auto global_best_move = moves[0];
    #pragma omp parallel
    {
        int local_best_score = first_player_turn ? INT_MIN : INT_MAX;
        auto local_best_move = moves[0];
        #pragma omp for schedule(static)
        for (int i = 0; i < moves.size(); ++i) {
            auto m = moves[i];
            Game game_copy = Game(game);
            game_copy.playMove(m);
            int score = 0;
            switch (game_copy.status()) {
                case Game::kFirstPlayerTurn:
                case Game::kSecondPlayerTurn:
                    if (plies == 0) {
                        score = game_copy.leafEvalState();
                    } else {
                        score = playBestMoveForGame(game_copy,
                                !first_player_turn, plies - 1);
                    }
                    break;
                case Game::kFirstPlayerWon:
                    score = INT_MAX;
                    break;
                case Game::kSecondPlayerWon:
                    score = INT_MIN;
                    break;
                case Game::kTie:
                    // TODO: Does this make sense? A tie is of neutral value?
                    score = 0;
                    break;
            }
            if (first_player_turn) {
                if (score > local_best_score) {
                    local_best_score = score;
                    local_best_move = m;
                }
            } else {
                if (score < local_best_score) {
                    local_best_score = score;
                    local_best_move = m;
                }
            }
        }
        if (first_player_turn) {
            #pragma omp critical
            {
                if (local_best_score > global_best_score) {
                    global_best_score = local_best_score;
                    global_best_move = local_best_move;
                }
            }
        }
        else {
            #pragma omp critical
            {
                if (local_best_score < global_best_score) {
                    global_best_score = local_best_score;
                    global_best_move = local_best_move;
                }
            }
        }
    }
    game.playMove(global_best_move);
    return global_best_score;
}

#endif  /* OMP_MINIMAX_LOCAL_SOLVER_H */
