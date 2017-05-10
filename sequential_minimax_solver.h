#ifndef SEQUENTIAL_MINIMAX_SOLVER_H
#define SEQUENTIAL_MINIMAX_SOLVER_H

#include "game_solver.h"

#include <climits>

// Uses sequential minimax to specified depth to evaluate moves. The minimum
// depth, 1, is equivalent to the HeuristicSolver.
template <class Game, int depth>
class SequentialMinimaxSolver : public GameSolver<Game> {
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
void SequentialMinimaxSolver<Game, depth>::playBestMove() {
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
int SequentialMinimaxSolver<Game, depth>::playBestMoveForGame(
        Game& game, bool first_player_turn, int plies) {
    auto moves = game.availableMoves();
    int best_score = first_player_turn ? INT_MIN : INT_MAX;
    auto best_move = moves[0];
    for (auto m : moves) {
        Game game_copy = Game(game);
        game_copy.playMove(m);
        int score = 0;
        switch (game_copy.status()) {
            case Game::kFirstPlayerTurn:
            case Game::kSecondPlayerTurn:
                if (plies == 0) {
                    score = game_copy.leafEvalState();
                } else {
                    score = playBestMoveForGame(game_copy, !first_player_turn,
                            plies - 1);
                }
                break;
            case Game::kFirstPlayerWon:
                // TODO: If first_player_turn, could break out of for loop here
                // because we know we can't do better than a winning move.
                score = INT_MAX;
                break;
            case Game::kSecondPlayerWon:
                // TODO: If !first_player_turn, could break out of for loop
                // here because we know we can't do better than a winning move.
                score = INT_MIN;
                break;
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
    }
    game.playMove(best_move);
    return best_score;
}

#endif  /* SEQUENTIAL_MINIMAX_SOLVER_H */
