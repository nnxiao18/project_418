#include "tic_tac_toe_game.h"

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
    const auto& board = current_state_.first;
    // Check for horizontal wins.
    for (int r = 0; r < BOARD_SIDE; ++r) {
        SquareStatus first = board[r * BOARD_SIDE];
        if (first != kEmpty) {
            bool won = true;
            for (int c = 1; c < BOARD_SIDE; ++c) {
                if (board[r * BOARD_SIDE + c] != first) {
                    won = false;
                    break;
                }
            }
            if (won) {
                return first == kX ? kFirstPlayerWon : kSecondPlayerWon;
            }
        }
    }
    // Check for vertical wins.
    for (int c = 0; c < BOARD_SIDE; ++c) {
        SquareStatus first = board[c];
        if (first != kEmpty) {
            bool won = true;
            for (int r = 1; r < BOARD_SIDE; ++r) {
                if (board[r * BOARD_SIDE + c] != first) {
                    won = false;
                    break;
                }
            }
            if (won) {
                return first == kX ? kFirstPlayerWon : kSecondPlayerWon;
            }
        }
    }
    // Check for \ diagonal win.
    SquareStatus first = board[0];
    if (first != kEmpty) {
        bool won = true;
        for (int rc = 1; rc < BOARD_SIDE; ++rc) {
            if (board[rc * BOARD_SIDE + rc] != first) {
                won = false;
                break;
            }
        }
        if (won) {
            return first == kX ? kFirstPlayerWon : kSecondPlayerWon;
        }
    }
    // Check for / diagonal win.
    first = board[BOARD_SIDE - 1];
    if (first != kEmpty) {
        bool won = true;
        for (int rc = 1; rc < BOARD_SIDE; ++rc) {
            if (board[rc * BOARD_SIDE + BOARD_SIDE - 1 - rc] != first) {
                won = false;
                break;
            }
        }
        if (won) {
            return first == kX ? kFirstPlayerWon : kSecondPlayerWon;
        }
    }
    // Check for empty squares.
    for (SquareStatus s : board) {
        if (s == kEmpty) {
            return current_state_.second ?
                    kFirstPlayerTurn : kSecondPlayerTurn;
        }
    }
    // If board is filled and no one won, it's a tie.
    return kTie;
}

// Give 3 points for center, 2 for corners, 1 for sides.
int TicTacToeGame::leafEvalState() const {
    int score = 0;
    for (int r = 0; r < BOARD_SIDE; ++r) {
        for (int c = 0; c < BOARD_SIDE; ++c) {
            int delta;
            if (r == 1 && c == 1) {
                // Center square.
                delta = 3;
            }
            else if (r - c == 1 || c - r == 1) {
                // Side (non-corner) square.
                delta = 1;
            }
            else {
                // Corner square.
                delta = 2;
            }
            switch (current_state_.first[r * BOARD_SIDE + c]) {
                case kEmpty:
                    break;
                case kX:
                    score += delta;
                    break;
                case kO:
                    score -= delta;
                    break;
            }
        }
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
