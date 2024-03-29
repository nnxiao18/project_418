#ifndef SEQUENTIAL_ALPHABETA_SOLVER_H
#define SEQUENTIAL_ALPHABETA_SOLVER_H

#include "game_solver.h"

#include <climits>

// Uses sequential minimax to specified depth with alpha-beta pruning to
// evaluate moves. The minimum depth, 1, is equivalent to the HeuristicSolver.
template <class Game, int depth>
class SequentialAlphaBetaSolver : public GameSolver<Game> {
  static_assert(depth >= 1, "Depth must be >= 1");
  using GameSolver<Game>::game_;
  private:
    // Finds the best move for game with minimax evaluation with alpha-beta
    // bounds up to plies, plays it, and returns its minimax value.
    int playBestMoveForGame(Game& game, bool first_player_turn, int plies, int
            alpha, int beta);
  public:
    virtual void playBestMove();
};

template <class Game, int depth>
void SequentialAlphaBetaSolver<Game, depth>::playBestMove() {
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
int SequentialAlphaBetaSolver<Game, depth>::playBestMoveForGame(
        Game& game, bool first_player_turn, int plies, int alpha, int beta) {
    auto moves = game.availableMoves();
    int best_score = first_player_turn ? INT_MIN : INT_MAX;
    auto best_move = moves[0];
    for (auto m : moves) {
        Game game_copy = Game(game);
        game_copy.playMove(m);
        int score = 0;
        bool found_winning_move = false;
        switch (game_copy.status()) {
            case Game::kFirstPlayerTurn:
            case Game::kSecondPlayerTurn:
                if (plies == 0) {
                    score = game_copy.leafEvalState();
                } else {
                    score = playBestMoveForGame(game_copy, !first_player_turn,
                            plies - 1, alpha, beta);
                }
                break;
            case Game::kFirstPlayerWon:
                best_score = INT_MAX;
                best_move = m;
                found_winning_move = true;
                break;
            case Game::kSecondPlayerWon:
                best_score = INT_MIN;
                best_move = m;
                found_winning_move = true;
                break;
            case Game::kTie:
                // TODO: Does this make sense? A tie is of neutral value?
                score = 0;
                break;

        }
        // If found_winning_move, no need to evaluate remanining moves.
        if (found_winning_move) {
            break;
        }
        if (first_player_turn) {
            if (score > best_score) {
                best_score = score;
                best_move = m;
            }
            if (score > alpha) {
                alpha = score;
                if (alpha > beta) {
                    break;  // Prune
                }
            }
        } else {
            if (score < best_score) {
                best_score = score;
                best_move = m;
            }
            if (score < beta) {
                beta = score;
                if (beta < alpha) {
                    break;  // Prune
                }
            }
        }
    }
    game.playMove(best_move);
    return best_score;
}

#endif  /* SEQUENTIAL_ALPHABETA_SOLVER_H */
