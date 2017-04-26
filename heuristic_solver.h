#ifndef HEURISTIC_SOLVER_H
#define HEURISTIC_SOLVER_H

#include "game_solver.h"

#include <climits>

// A basic heuristic solver to demonstrate + test how to create a GameSolver.
// Simply plays all available moves, ranks them by leaf evaluation on the
// resulting states, and plays the one yielding the highest (or lowest) score.
template <class Game>
class HeuristicSolver : public GameSolver<Game>{
  using GameSolver<Game>::game_;
  public:
    virtual void playBestMove();
};

template <class Game>
void HeuristicSolver<Game>::playBestMove() {
    bool first_player_turn = true;
    // If game is over, return without action.
    switch (game_.status()) {
        case Game::kFirstPlayerWon:
        case Game::kSecondPlayerWon:
        case Game::kTie:
            return;
        case Game::kFirstPlayerTurn:
            first_player_turn = true;
            break;
        case Game::kSecondPlayerTurn:
            first_player_turn = false;
            break;
    }
    auto moves = game_.availableMoves();
    int best_score = first_player_turn ? INT_MIN : INT_MAX;
    auto best_move = moves[0];
    for (auto m : moves) {
        Game game_copy = Game(game_);
        game_copy.playMove(m);
        int score = 0;
        switch (game_copy.status()) {
            case Game::kFirstPlayerTurn:
            case Game::kSecondPlayerTurn:
                score = game_copy.leafEvalState();
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
    game_.playMove(best_move);
    return;
}

#endif  /* HEURISTIC_SOLVER_H */
