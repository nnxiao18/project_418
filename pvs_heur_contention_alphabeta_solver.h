#ifndef PVS_HEUR_CONTENTION_ALPHABETA_SOLVER_H
#define PVS_HEUR_CONTENTION_ALPHABETA_SOLVER_H

#include "game_solver.h"

#include <iostream>
#include <climits>

// Uses top-level parallel (OpenMP) minimax to specified depth with
// alpha-beta pruning to evaluate moves, updating thread-local alpha-beta
// values to reduce contention. No-copy. PVS with heuristic selection of PV.
template <class Game, int depth>
class PVSHeurContentionAlphaBetaSolver : public GameSolver<Game> {
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
void PVSHeurContentionAlphaBetaSolver<Game, depth>::playBestMove() {
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
int PVSHeurContentionAlphaBetaSolver<Game, depth>::playBestMoveForGame(
        Game& game, bool first_player_turn, int plies, int alpha, int beta) {

    auto moves = game.availableMoves();
    // First find PV (best move) according to leaf heuristic.
    int heur_score = 0;
    int heur_move_index = 0;
    for (size_t i = 0; i < moves.size(); ++i) {
        auto m = moves[i];
        game.playMove(m);
        int score = game.leafEvalState();
        if (score > heur_score) {
            heur_score = score;
            heur_move_index = i;
        }
        game.undoMove(m);
    }

    game.playMove(moves[heur_move_index]);
    auto best_move = moves[heur_move_index];
    int best_score;
    if (plies == 0) {
        best_score = game.leafEvalState();
    } else {
        best_score = PVSplit(game, !first_player_turn, plies-1, alpha, beta);
    }
    if (first_player_turn) {
        if (best_score > alpha) alpha = best_score;
    } else {
        if (best_score < beta) beta = best_score;
    }

    game.undoMove(moves[heur_move_index]);
    // Efficiently remove the PV move to avoid recomputing it.
    std::swap(moves[heur_move_index], moves.back());
    moves.pop_back();

    bool found_winning_move = false;
    #pragma omp parallel
    {
        Game local_game = Game(game);
        #pragma omp for schedule(static)
        for (int i = 0; i < moves.size(); ++i) {
            if (found_winning_move){ continue; }
            const auto& m = moves[i];
            local_game.playMove(m);
            int score = 0;
            switch (game.status()) {
                case Game::kFirstPlayerTurn:
                case Game::kSecondPlayerTurn:
                    if (plies == 0) {
                        score = local_game.leafEvalState();
                    } else {
                        score = evalState(local_game, !first_player_turn,
                                plies - 1, alpha, beta);
                    }
                    break;
                case Game::kFirstPlayerWon:
                    best_score = INT_MAX;
                    found_winning_move = true;
                    best_move = m;
                    continue;
                case Game::kSecondPlayerWon:
                    best_score = INT_MIN;
                    found_winning_move = true;
                    best_move = m;
                    continue;
                case Game::kTie:
                    // TODO: Does this make sense? A tie is of neutral value?
                    score = 0;
                    break;
            }
            if (first_player_turn){
                #pragma omp critical
                {
                    if (score > best_score){
                        best_score = score;
                        best_move = m;
                        alpha = best_score;
                    }
                }
            } else {
                #pragma omp critical
                {
                    if (score < best_score){
                        best_score = score;
                        best_move = m;
                        beta = best_score;
                    }
                }
            }
            local_game.undoMove(m);
        }
    }
    game.playMove(best_move);
    return best_score;
}

template <class Game, int depth>
int PVSHeurContentionAlphaBetaSolver<Game, depth>::evalState(
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

template <class Game, int depth>
int PVSHeurContentionAlphaBetaSolver<Game, depth>::PVSplit(
        Game& game, bool first_player_turn, int plies, int alpha, int beta) {

    auto moves = game.availableMoves();
    // First find PV (best move) according to leaf heuristic.
    int heur_score = 0;
    int heur_move_index = 0;
    for (size_t i = 0; i < moves.size(); ++i) {
        auto m = moves[i];
        game.playMove(m);
        int score = game.leafEvalState();
        if (score > heur_score) {
            heur_score = score;
            heur_move_index = i;
        }
        game.undoMove(m);
    }

    game.playMove(moves[heur_move_index]);
    int best_score;
    if (plies == 0){
        best_score = game.leafEvalState();
    } else {
        best_score = PVSplit(game, !first_player_turn, plies-1, alpha, beta);
    }
    if (best_score > beta) {
        return beta;
    } else if (best_score < alpha) {
        return alpha;
    }
    if (first_player_turn) {
        if (best_score > alpha) alpha = best_score;
    } else {
        if (best_score < beta) beta = best_score;
    }

    game.undoMove(moves[heur_move_index]);
    // Efficiently remove the PV move to avoid recomputing it.
    std::swap(moves[heur_move_index], moves.back());
    moves.pop_back();

    bool found_winning_move = false;
    #pragma omp parallel
    {
        Game local_game = Game(game);
        #pragma omp for schedule(static)
        for (size_t i = 0; i < moves.size(); ++i){
            if(found_winning_move){ continue; }
            const auto& m = moves[i];
            local_game.playMove(m);
            int score = 0;
           switch (local_game.status()){
                case Game::kFirstPlayerTurn:
                case Game::kSecondPlayerTurn:
                    if (plies == 0){
                        score = local_game.leafEvalState();
                    } else {
                        score = evalState(local_game, !first_player_turn, plies-1, alpha, beta);
                    }
                    break;
                case Game::kFirstPlayerWon:
                    local_game.undoMove(m);
                    best_score = INT_MAX;
                    found_winning_move = true;
                    continue;
                case Game::kSecondPlayerWon:
                    local_game.undoMove(m);
                    best_score = INT_MIN;
                    found_winning_move = true;
                    continue;
                case Game::kTie:
                    score = 0;
                    break;
            }
            if (first_player_turn){
                #pragma omp critical
                {
                    if (score > best_score){
                        best_score = score;
                        alpha = best_score;
                    }
                }
            } else {
                #pragma omp critical
                {
                    if (score < best_score){
                        best_score = score;
                        beta = best_score;
                    }
                }
            }
            local_game.undoMove(m);
        }
    }
    return best_score;
}

#endif  /* PVS_HEUR_CONTENTION_ALPHABETA_SOLVER_H */
