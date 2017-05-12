#include "gomoku_game.h"

#include <climits>
#include <iostream>

std::vector<GomokuMove> GomokuGame::availableMoves() const {
    std::vector<GomokuMove> moves;
    for (int r = 0; r < BOARD_SIDE; ++r) {
        for (int c = 0; c < BOARD_SIDE; ++c) {
            if (current_state_.first[r * BOARD_SIDE + c] == kEmpty) {
                moves.push_back(r * BOARD_SIDE + c);
            }
        }
    }
    return moves;
}

GomokuGame::GameStatus GomokuGame::status() const {
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

int GomokuGame::leafEvalState() const {
    const auto& board = current_state_.first;
    int total_score = 0;
    // For every square in the board...
    for (int r = 0; r < BOARD_SIDE; ++r) {
        for (int c = 0; c < BOARD_SIDE; ++c) {
            int multiplier = 0;
            SquareStatus curr_square = board[r * BOARD_SIDE + c];
            switch (curr_square) {
                case kX:
                    multiplier = 1;
                    break;
                case kO:
                    multiplier = -1;
                    break;
                default:
                    break;
            }
            // If empty, move on.
            if (multiplier == 0) {
                continue;
            }
            // For each of 8 possible directions (excludes (0,0))...
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }
                    int lane_end_r = r + (K_IN_ROW * dx);
                    int lane_end_c = c + (K_IN_ROW * dy);
                    if (lane_end_r < -1 || lane_end_r > BOARD_SIDE ||
                        lane_end_c < -1 || lane_end_c > BOARD_SIDE) {
                        // Cannot make a K_IN_ROW in this direction.
                        continue;
                    }
                    int lane_score = 1;
                    bool blocked = false;
                    // For each square within K_IN_ROW in that direction...
                    for (int m = 1; m < K_IN_ROW; ++m) {
                        int rr = r + m * dx;
                        int cc = c + m * dy;
                        SquareStatus lane_square = board[rr * BOARD_SIDE + cc];
                        if (lane_square != kEmpty) {
                            if (lane_square != curr_square) {
                                blocked = true;
                                break;
                            }
                            else {
                                lane_score += 1;
                            }
                        }
                    }
                    if (lane_score == K_IN_ROW) {
                        // Someone won!
                        return curr_square == kX ? INT_MAX : INT_MIN;
                    }
                    if (!blocked) {
                        total_score += multiplier * lane_score;
                    }
                }
            }
        }
    }
    return total_score;
}

void GomokuGame::playMove(GomokuMove move) {
    if (current_state_.second) {
        current_state_.first[move] = kX;
    }
    else {
        current_state_.first[move] = kO;
    }
    current_state_.second = !current_state_.second;
}

void GomokuGame::resetState() {
    current_state_.first.fill(kEmpty);
    /*
    current_state_.first[3 * BOARD_SIDE + 6]=kX;

    current_state_.first[4 * BOARD_SIDE + 4]=kO;
    current_state_.first[4 * BOARD_SIDE + 7]=kO;

    current_state_.first[5 * BOARD_SIDE + 3]=kO;
    current_state_.first[5 * BOARD_SIDE + 4]=kX;
    current_state_.first[5 * BOARD_SIDE + 6]=kO;
    current_state_.first[5 * BOARD_SIDE + 7]=kX;
    current_state_.first[5 * BOARD_SIDE + 8]=kO;
    current_state_.first[5 * BOARD_SIDE + 9]=kX;

    current_state_.first[6 * BOARD_SIDE + 4]=kX;
    current_state_.first[6 * BOARD_SIDE + 5]=kO;
    current_state_.first[6 * BOARD_SIDE + 7]=kX;
    current_state_.first[6 * BOARD_SIDE + 8]=kX;
    current_state_.first[6 * BOARD_SIDE + 9]=kO;

    current_state_.first[7 * BOARD_SIDE + 4]=kX;
    current_state_.first[7 * BOARD_SIDE + 5]=kO;
    current_state_.first[7 * BOARD_SIDE + 6]=kX;
    current_state_.first[7 * BOARD_SIDE + 7]=kX;
    current_state_.first[7 * BOARD_SIDE + 8]=kX;
    current_state_.first[7 * BOARD_SIDE + 9]=kO;

    current_state_.first[8 * BOARD_SIDE + 5]=kX;
    current_state_.first[8 * BOARD_SIDE + 6]=kO;
    current_state_.first[8 * BOARD_SIDE + 8]=kO;
    current_state_.first[8 * BOARD_SIDE + 9]=kX;

    current_state_.first[9 * BOARD_SIDE + 4]=kO;
    current_state_.first[9 * BOARD_SIDE + 7]=kO;

    current_state_.first[10 * BOARD_SIDE + 6]=kX;

    current_state_.first[11 * BOARD_SIDE + 5]=kO;
*/
    current_state_.second = true;
}

void GomokuGame::undoMove(GomokuMove move) {
    current_state_.first[move] = kEmpty;
    current_state_.second = !current_state_.second;
}

std::string GomokuGame::stateString() const {
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
