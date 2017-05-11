#include "tic_tac_toe_game.h"

#include <climits>
#include <iostream>

std::vector<TicTacToeMove> TicTacToeGame::availableMoves() const {
    std::vector<TicTacToeMove> moves;
    for (int r = 0; r < BOARD_SIDE; ++r) {
        for (int c = 0; c < BOARD_SIDE; ++c) {
            if (current_state_.first[r * BOARD_SIDE + c] == kEmpty) {
                moves.push_back(r * BOARD_SIDE + c);
            }
        }
    }
    return moves;
}

TicTacToeGame::GameStatus TicTacToeGame::status() const {
    int leafEvalScore = leafEvalState();
    if (leafEvalScore == INT_MAX) {
        return kFirstPlayerWon;
    }
    else if (leafEvalScore == INT_MIN) {
        return kSecondPlayerWon;
    }
    // Check for empty squares.
    for (SquareStatus s : current_state_.first) {
        if (s == kEmpty) {
            return current_state_.second ?
                    kFirstPlayerTurn : kSecondPlayerTurn;
        }
    }
    // If board is filled and no one won, it's a tie.
    return kTie;
}

int TicTacToeGame::leafEvalState() const {
    const auto& board = current_state_.first;
    int score = 0;
    // Score rows.
    for (int r = 0; r < BOARD_SIDE; ++r) {
        int count_x = 0;
        int count_o = 0;
        for (int c = 0; c < BOARD_SIDE; ++c) {
            switch (board[r * BOARD_SIDE + c]) {
                case kX:
                    ++count_x;
                    break;
                case kO:
                    ++count_o;
                    break;
                default:
                    break;
            }
        }
        if (count_o == 0) {
            if (count_x == BOARD_SIDE) {
                return INT_MAX;
            }
            score += count_x;
        }
        else if (count_x == 0) {
            if (count_o == BOARD_SIDE) {
                return INT_MIN;
            }
            score -= count_o;
        }
    }
    // Score columns.
    for (int c = 0; c < BOARD_SIDE; ++c) {
        int count_x = 0;
        int count_o = 0;
        for (int r = 0; r < BOARD_SIDE; ++r) {
            switch (board[r * BOARD_SIDE + c]) {
                case kX:
                    ++count_x;
                    break;
                case kO:
                    ++count_o;
                    break;
                default:
                    break;
            }
        }
        if (count_o == 0) {
            if (count_x == BOARD_SIDE) {
                return INT_MAX;
            }
            score += count_x;
        }
        else if (count_x == 0) {
            if (count_o == BOARD_SIDE) {
                return INT_MIN;
            }
            score -= count_o;
        }
    }
    // Score \ diagonal.
    int count_x = 0;
    int count_o = 0;
    for (int rc = 0; rc < BOARD_SIDE; ++rc) {
        switch (board[rc * BOARD_SIDE + rc]) {
            case kX:
                ++count_x;
                break;
            case kO:
                ++count_o;
                break;
            default:
                break;
        }
    }
    if (count_o == 0) {
        if (count_x == BOARD_SIDE) {
            return INT_MAX;
        }
        score += count_x;
    }
    else if (count_x == 0) {
        if (count_o == BOARD_SIDE) {
            return INT_MIN;
        }
        score -= count_o;
    }
    // Score / diagonal.
    count_x = 0;
    count_o = 0;
    for (int rc = 0; rc < BOARD_SIDE; ++rc) {
        switch (board[rc * BOARD_SIDE + BOARD_SIDE - 1 - rc]) {
            case kX:
                ++count_x;
                break;
            case kO:
                ++count_o;
                break;
            default:
                break;
        }
    }
    if (count_o == 0) {
        if (count_x == BOARD_SIDE) {
            return INT_MAX;
        }
        score += count_x;
    }
    else if (count_x == 0) {
        if (count_o == BOARD_SIDE) {
            return INT_MIN;
        }
        score -= count_o;
    }
    return score;
}

void TicTacToeGame::playMove(TicTacToeMove move) {
    if (current_state_.second) {
        current_state_.first[move] = kX;
    }
    else {
        current_state_.first[move] = kO;
    }
    current_state_.second = !current_state_.second;
}

void TicTacToeGame::resetState() {
    std::array<SquareStatus, BOARD_LEN> init_board;
    init_board.fill(kEmpty);
    current_state_ = std::make_pair(std::move(init_board), true);
    return;
}

void TicTacToeGame::undoMove(TicTacToeMove move) {
    current_state_.first[move] = kEmpty;
    current_state_.second = !current_state_.second;
}

std::string TicTacToeGame::stateString() const {
    std::string state_string;
    if (current_state_.second) {
        state_string.append("First player's turn\n");
    }
    else {
        state_string.append("Second player's turn\n");
    }
    for (int r = 0; r < BOARD_SIDE; ++r) {
        for (int c = 0; c < BOARD_SIDE; ++c) {
            switch (current_state_.first[r * BOARD_SIDE + c]) {
                case kEmpty:
                    state_string.append(" ");
                    break;
                case kX:
                    state_string.append("X");
                    break;
                case kO:
                    state_string.append("O");
                    break;
            }
            state_string.append(" ");
        }
        state_string.append("\n");
    }
    return state_string;
}
