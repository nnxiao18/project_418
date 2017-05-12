#ifndef PVS_LOCAL_ALPHABETA_SOLVER_H
#define PVS_LOCAL_ALPHABETA_SOLVER_H

#include "game_solver.h"

#include <iostream>
#include <climits>

// Uses top-level parallel (OpenMP) minimax to specified depth with
// alpha-beta pruning to evaluate moves, updating thread-local alpha-beta
// values to reduce contention. No-copy. PVS
template <class Game, int depth>
class PVSLocalAlphaBetaSolver : public GameSolver<Game> {
  static_assert(depth >= 1, "Depth must be >= 1");
  using GameSolver<Game>::game_;
  private:
    // Finds the best move for game with minimax evaluation with alpha-beta
    // bounds up to plies, plays it, and returns its minimax value.
    int playBestMoveForGame(Game& game, bool first_player_turn, int plies, int
            alpha, int beta);
    int evalState(Game& game, bool first_player_turn, int plies, int alpha, int
            beta);
    int PVSplit(Game& game, bool first_player_turn, int plies, int alpha, int beta);
  public:
    virtual void playBestMove();
};

template <class Game, int depth>
void PVSLocalAlphaBetaSolver<Game, depth>::playBestMove() {
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
int PVSLocalAlphaBetaSolver<Game, depth>::playBestMoveForGame(
        Game& game, bool first_player_turn, int plies, int alpha, int beta) {
    auto moves = game.availableMoves();
    //std::cout << "playBestMoveForGame plies: " << plies << "\n";
    auto global_best_move = moves[0];
    game.playMove(global_best_move);
    int global_best_score;
    if (plies == 0) {
        global_best_score = game.leafEvalState();
    } else {
        global_best_score = PVSplit(game, first_player_turn, plies-1, alpha, beta);
    }
    //printf("huh\n");
    if (first_player_turn) {
        if (global_best_score > alpha) alpha = global_best_score;
    } else {
        if (global_best_score < beta) beta = global_best_score;
    }

    game.undoMove(moves[0]);
    bool found_winning_move = false;
    #pragma omp parallel
    {
        int local_best_score = first_player_turn ? INT_MIN : INT_MAX;
        auto local_best_move = moves[1];
        int local_alpha = alpha;
        int local_beta = beta;
        Game local_game = Game(game);
        #pragma omp for schedule(static)
        for (int i = 1; i < moves.size(); ++i) {
            if (found_winning_move) { continue;}
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
                    found_winning_move = true;
                    global_best_move = m;
                    continue;
                case Game::kSecondPlayerWon:
                    global_best_score = INT_MIN;
                    found_winning_move = true;
                    global_best_move = m;
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
int PVSLocalAlphaBetaSolver<Game, depth>::evalState(
        Game& game, bool first_player_turn, int plies, int alpha, int beta) {
    auto moves = game.availableMoves();
    //std::cout << "evalState plies: " << plies << "\n";
    int best_score = first_player_turn ? INT_MIN : INT_MAX;
    //printf("a\n");
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

template <class Game, int depth>
int PVSLocalAlphaBetaSolver<Game, depth>::PVSplit(
        Game& game, bool first_player_turn, int plies, int alpha, int beta) {

    //std::cout << "PVSplit plies: " << plies << "\n";
    auto moves = game.availableMoves();
    if (moves.size() == 0)
        return game.leafEvalState();

    game.playMove(moves[0]);
    int global_best_score;
    if (plies == 0){
        global_best_score = game.leafEvalState();
    } else {
        global_best_score = PVSplit(game, !first_player_turn, plies-1, alpha, beta);
    }
    if (global_best_score > beta)
        return beta;
    if (global_best_score > alpha)
        alpha = global_best_score;

    game.undoMove(moves[0]);
    bool found_winning_move = false;
    #pragma omp parallel
    {
        int local_best_score = first_player_turn ? INT_MIN : INT_MAX;
        int local_alpha = alpha;
        int local_beta = beta;
        Game local_game = Game(game);
        #pragma omp for schedule(static)
        for (size_t i = 1; i < moves.size(); ++i){
            if (found_winning_move) {continue;}
            //printf("i:%zu, size: %lu\n",i,moves.size());
            const auto& m = moves[i];
            local_game.playMove(m);
            int score = 0;
           switch (local_game.status()){
                case Game::kFirstPlayerTurn:
                case Game::kSecondPlayerTurn:
                    if (plies == 0){
                        score = local_game.leafEvalState();
                    } else {
                        score = evalState(local_game, !first_player_turn, plies-1, local_alpha, local_beta);
                    }
                    break;
                case Game::kFirstPlayerWon:
                    local_game.undoMove(m);
                    global_best_score = INT_MAX;
                    found_winning_move = true;
                    continue;
                case Game::kSecondPlayerWon:
                    local_game.undoMove(m);
                    global_best_score = INT_MIN;
                    found_winning_move = true;
                    continue;
                case Game::kTie:
                    score = 0;
                    break;
            }
            if (first_player_turn){
                if (score > local_best_score){
                    local_best_score = score;
                    local_alpha = local_best_score;
                }
            } else {
                if (score < local_best_score){
                    local_best_score = score;
                    local_beta = local_best_score;
                }
            }
            local_game.undoMove(m);
        }
        if (first_player_turn){
            #pragma omp critical
            {
                if (local_best_score > global_best_score){
                    global_best_score = local_best_score;
                }
            }
        } else {
            #pragma omp critical
            {
                if (local_best_score < global_best_score){
                    global_best_score = local_best_score;
                }
            }
        }
    }
    return global_best_score;
}
#endif  /* PVS_LOCAL_ALPHABETA_SOLVER_H */
