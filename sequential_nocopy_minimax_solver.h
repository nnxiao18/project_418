#ifndef SEQUENTIAL_NOCOPY_MINIMAX_SOLVER_H
#define SEQUENTIAL_NOCOPY_MINIMAX_SOLVER_H

#include "game_solver.h"

#include <climits>

// Uses sequential minimax to specified depth to evaluate moves. Undoes moves
// instead of making game copies for memory efficiency. The minimum
// depth, 1, is equivalent to the HeuristicSolver.
template <class Game, int depth>
class SequentialNoCopyMinimaxSolver : public GameSolver<Game> {
  static_assert(depth >= 1, "Depth must be >= 1");
  using GameSolver<Game>::game_;
  private:
    // Finds the best move for game with minimax evaluation up to plies, plays
    // it, and returns its minimax value.
    int playBestMoveForGame(Game& game, bool first_player_turn, int plies);
    int evalState(Game& game, bool first_player_turn, int plies);
  public:
    virtual void playBestMove();
};

template <class Game, int depth>
void SequentialNoCopyMinimaxSolver<Game, depth>::playBestMove() {
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
int SequentialNoCopyMinimaxSolver<Game, depth>::playBestMoveForGame(
        Game& game, bool first_player_turn, int plies) {
    auto moves = game.availableMoves();
    int best_score = first_player_turn ? INT_MIN : INT_MAX;
    auto best_move = moves[0];
    for (auto m : moves) {
        game.playMove(m);
        int score = 0;
        switch (game.status()) {
            case Game::kFirstPlayerTurn:
            case Game::kSecondPlayerTurn:
                if (plies == 0) {
                    score = game.leafEvalState();
                } else {
                    score = evalState(game, !first_player_turn, plies - 1);
                }
                break;
            case Game::kFirstPlayerWon:
                // Leave the move done; we want to do this winning move.
                return INT_MAX;
            case Game::kSecondPlayerWon:
                // Leave the move done; we want to do this winning move.
                return INT_MIN;
            case Game::kTie:
                // TODO: Does this make sense? A tie is of neutral value?
                score = 0;
                break;
        }
        if (first_player_turn) {
            if (score > best_score) {
                best_score = score;
                best_move = m;
            }
        } else {
            if (score < best_score) {
                best_score = score;
                best_move = m;
            }
        }
        game.undoMove(m);
    }
    game.playMove(best_move);
    return best_score;
}

template <class Game, int depth>
int SequentialNoCopyMinimaxSolver<Game, depth>::evalState(
        Game& game, bool first_player_turn, int plies) {
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
                    score = evalState(game, !first_player_turn, plies - 1);
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
        } else {
            if (score < best_score) {
                best_score = score;
            }
        }
        game.undoMove(m);
    }
    return best_score;
}

#endif  /* SEQUENTIAL_NOCOPY_MINIMAX_SOLVER_H */
