#ifndef OMP_LOCAL_ALPHABETA_SOLVER_H
#define OMP_LOCAL_ALPHABETA_SOLVER_H

#include "game_solver.h"

#include <climits>

// Uses top-level parallel (OpenMP) minimax to specified depth with
// alpha-beta pruning to evaluate moves, updating thread-local alpha-beta
// values to reduce contention. No-copy.
template <class Game, int depth>
class OmpLocalAlphaBetaSolver : public GameSolver<Game> {
  static_assert(depth >= 1, "Depth must be >= 1");
  using GameSolver<Game>::game_;
  private:
    // Finds the best move for game with minimax evaluation with alpha-beta
    // bounds up to plies, plays it, and returns its minimax value.
    int playBestMoveForGame(Game& game, bool first_player_turn, int plies, int
            alpha, int beta);
    int evalState(Game& game, bool first_player_turn, int plies, int alpha, int
            beta);
  public:
    virtual void playBestMove();
};

template <class Game, int depth>
void OmpLocalAlphaBetaSolver<Game, depth>::playBestMove() {
    switch (game_.status()) {
        case Game::kFirstPlayerWon:
        case Game::kSecondPlayerWon:
        case Game::kTie:
            // If game is over, return without action.
            return;
        case Game::kFirstPlayerTurn:
            playBestMoveForGame(game_, true, depth - 1, INT_MIN, INT_MAX);
            break;
        case Game::kSecondPlayerTurn:
            playBestMoveForGame(game_, false, depth - 1, INT_MIN, INT_MAX);
            break;
    }
}

template <class Game, int depth>
int OmpLocalAlphaBetaSolver<Game, depth>::playBestMoveForGame(
        Game& game, bool first_player_turn, int plies, int alpha, int beta) {
    auto moves = game.availableMoves();
    int global_best_score = first_player_turn ? INT_MIN : INT_MAX;
    auto global_best_move = moves[0];
    bool found_winning_move = false;
    #pragma omp parallel
    {
        int local_best_score = first_player_turn ? INT_MIN : INT_MAX;
        auto local_best_move = moves[0];
        int local_alpha = alpha;
        int local_beta = beta;
        Game local_game = Game(game);
        #pragma omp for schedule(static)
        for (size_t i = 0; i < moves.size(); ++i) {
            if (found_winning_move) {
                continue;
            }
            const auto& m = moves[i];
            local_game.playMove(m);
            int score = 0;
            switch (local_game.status()) {
                case Game::kFirstPlayerTurn:
                case Game::kSecondPlayerTurn:
                    if (plies == 0) {
                        score = local_game.leafEvalState();
                    } else {
                        score = evalState(local_game, !first_player_turn,
                                plies - 1, local_alpha, local_beta);
                    }
                    break;
                case Game::kFirstPlayerWon:
                    global_best_score = INT_MAX;
                    global_best_move = m;
                    found_winning_move = true;
                    // No need for local_game.undoMove(m); we won't be trying
                    // any other moves on it.
                    continue;
                case Game::kSecondPlayerWon:
                    global_best_score = INT_MIN;
                    global_best_move = m;
                    found_winning_move = true;
                    // No need for local_game.undoMove(m); we won't be trying
                    // any other moves on it.
                    continue;
                case Game::kTie:
                    // TODO: Does this make sense? A tie is of neutral value?
                    score = 0;
                    break;
            }
            if (first_player_turn) {
                if (score > local_best_score) {
                    local_best_score = score;
                    local_best_move = m;
                    local_alpha = local_best_score;
                    // At top level, don't have to consider pruning.
                }
            } else {
                if (score < local_best_score) {
                    local_best_score = score;
                    local_best_move = m;
                    local_beta = local_best_score;
                    // At top level, don't have to consider pruning.
                }
            }
            local_game.undoMove(m);
        }
        // Accumulate local best scores and moves to find global.
        if (first_player_turn) {
            #pragma omp critical
            {
                if (local_best_score > global_best_score) {
                    global_best_score = local_best_score;
                    global_best_move = local_best_move;
                }
            }
        } else {
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

template <class Game, int depth>
int OmpLocalAlphaBetaSolver<Game, depth>::evalState(
        Game& game, bool first_player_turn, int plies, int alpha, int beta) {
    auto moves = game.availableMoves();
    int best_score = first_player_turn ? INT_MIN : INT_MAX;
    for (auto m : moves) {
        game.playMove(m);
        int score = 0;
        switch (game.status()) {
            case Game::kFirstPlayerTurn:
            case Game::kSecondPlayerTurn:
                if (plies == 0) {
                    score = game.leafEvalState();
                } else {
                    score = evalState(game, !first_player_turn, plies - 1,
                            alpha, beta);
                }
                break;
            case Game::kFirstPlayerWon:
                game.undoMove(m);
                return INT_MAX;
            case Game::kSecondPlayerWon:
                game.undoMove(m);
                return INT_MIN;
            case Game::kTie:
                // TODO: Does this make sense? A tie is of neutral value?
                score = 0;
                break;
        }
        if (first_player_turn) {
            if (score > best_score) {
                best_score = score;
            }
            if (score > alpha) {
                alpha = score;
                if (alpha > beta) {
                    game.undoMove(m);
                    break;  // Prune
                }
            }
        } else {
            if (score < best_score) {
                best_score = score;
            }
            if (score < beta) {
                beta = score;
                if (beta < alpha) {
                    game.undoMove(m);
                    break;  // Prune
                }
            }
        }
        game.undoMove(m);
    }
    return best_score;
}

#endif  /* OMP_LOCAL_ALPHABETA_SOLVER_H */
