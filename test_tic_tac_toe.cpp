#include <cassert>
#include <iostream>

#include "tic_tac_toe_game.h"

#define XWV_INITIAL_STATE "First player's turn\n      \n      \n      \n"
#define XWV_TURN_1 "Second player's turn\nX     \n      \n      \n"
#define XWV_TURN_2 "First player's turn\nX     \n  O   \n      \n"
#define XWV_TURN_3 "Second player's turn\nX     \n  O   \nX     \n"
#define XWV_TURN_4 "First player's turn\nX O   \n  O   \nX     \n"
#define XWV_TURN_5 "Second player's turn\nX O   \nX O   \nX     \n"

std::vector<TicTacToeMove> all_moves = {0, 1, 2, 3, 4, 5, 6, 7, 8};

void test_x_win_vertical() {
    // Initialize game.
    TicTacToeGame g;
    assert(g.isOver() == TicTacToeGame::kNotOver);
    std::string current_state = g.stateString();
    assert(current_state == XWV_INITIAL_STATE);
    assert(g.availableMoves() == all_moves);
    // Play X at (0,0).
    g.playMove(0);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == XWV_TURN_1);
    // Play O at (1,1).
    g.playMove(4);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == XWV_TURN_2);
    // Play X at (2,0).
    g.playMove(6);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == XWV_TURN_3);
    // Play O at (0,1).
    g.playMove(1);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == XWV_TURN_4);
    // Play X at (1,0) to win.
    g.playMove(3);
    assert(g.isOver() == TicTacToeGame::kFirstPlayerWon);
    current_state = g.stateString();
    assert(current_state == XWV_TURN_5);
    assert(g.leafEvalState() == 1);
    std::vector<TicTacToeMove> end_moves = {2, 5, 7, 8};
    assert(g.availableMoves() == end_moves);
}

#define OWH_INITIAL_STATE "First player's turn\n      \n      \n      \n"
#define OWH_TURN_1 "Second player's turn\nX     \n      \n      \n"
#define OWH_TURN_2 "First player's turn\nX     \n  O   \n      \n"
#define OWH_TURN_3 "Second player's turn\nX     \n  O   \nX     \n"
#define OWH_TURN_4 "First player's turn\nX     \nO O   \nX     \n"
#define OWH_TURN_5 "Second player's turn\nX     \nO O   \nX X   \n"
#define OWH_TURN_6 "First player's turn\nX     \nO O O \nX X   \n"

void test_o_win_horizontal() {
    // Initialize game.
    TicTacToeGame g;
    assert(g.isOver() == TicTacToeGame::kNotOver);
    std::string current_state = g.stateString();
    assert(current_state == OWH_INITIAL_STATE);
    // Play X at (0,0).
    g.playMove(0);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == OWH_TURN_1);
    // Play O at (1,1).
    g.playMove(4);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == OWH_TURN_2);
    // Play X at (2,0).
    g.playMove(6);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == OWH_TURN_3);
    // Play O at (1,0).
    g.playMove(3);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == OWH_TURN_4);
    // Play X at (2,1).
    g.playMove(7);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == OWH_TURN_5);
    // Play O at (1,2) to win.
    g.playMove(5);
    assert(g.isOver() == TicTacToeGame::kSecondPlayerWon);
    current_state = g.stateString();
    assert(current_state == OWH_TURN_6);
    assert(g.leafEvalState() == 0);
    std::vector<TicTacToeMove> end_moves = {1, 2, 8};
    assert(g.availableMoves() == end_moves);
}

#define XWD1_INITIAL_STATE "First player's turn\n      \n      \n      \n"
#define XWD1_TURN_1 "Second player's turn\nX     \n      \n      \n"
#define XWD1_TURN_2 "First player's turn\nX     \n      \nO     \n"
#define XWD1_TURN_3 "Second player's turn\nX     \n  X   \nO     \n"
#define XWD1_TURN_4 "First player's turn\nX     \n  X   \nO O   \n"
#define XWD1_TURN_5 "Second player's turn\nX     \n  X   \nO O X \n"

void test_x_win_diagonal1() {
    // Initialize game.
    TicTacToeGame g;
    assert(g.isOver() == TicTacToeGame::kNotOver);
    std::string current_state = g.stateString();
    assert(current_state == XWD1_INITIAL_STATE);
    // Play X at (0,0).
    g.playMove(0);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == XWD1_TURN_1);
    // Play O at (2,0).
    g.playMove(6);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == XWD1_TURN_2);
    // Play X at (1,1).
    g.playMove(4);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == XWD1_TURN_3);
    // Play O at (2,1).
    g.playMove(7);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == XWD1_TURN_4);
    // Play X at (2,2).
    g.playMove(8);
    assert(g.isOver() == TicTacToeGame::kFirstPlayerWon);
    current_state = g.stateString();
    assert(current_state == XWD1_TURN_5);
    assert(g.leafEvalState() == 4);
    std::vector<TicTacToeMove> end_moves = {1, 2, 3, 5};
    assert(g.availableMoves() == end_moves);
}

#define OWD2_INITIAL_STATE "First player's turn\n      \n      \n      \n"
#define OWD2_TURN_1 "Second player's turn\nX     \n      \n      \n"
#define OWD2_TURN_2 "First player's turn\nX     \n  O   \n      \n"
#define OWD2_TURN_3 "Second player's turn\nX     \nX O   \n      \n"
#define OWD2_TURN_4 "First player's turn\nX     \nX O   \nO     \n"
#define OWD2_TURN_5 "Second player's turn\nX X   \nX O   \nO     \n"
#define OWD2_TURN_6 "First player's turn\nX X O \nX O   \nO     \n"

void test_o_win_diagonal2() {
    // Initialize game.
    TicTacToeGame g;
    assert(g.isOver() == TicTacToeGame::kNotOver);
    std::string current_state = g.stateString();
    assert(current_state == OWD2_INITIAL_STATE);
    // Play X at (0,0).
    g.playMove(0);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == OWD2_TURN_1);
    // Play O at (1,1).
    g.playMove(4);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == OWD2_TURN_2);
    // Play X at (1,0).
    g.playMove(3);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == OWD2_TURN_3);
    // Play O at (2,0).
    g.playMove(6);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == OWD2_TURN_4);
    // Play X at (0,1).
    g.playMove(1);
    assert(g.isOver() == TicTacToeGame::kNotOver);
    current_state = g.stateString();
    assert(current_state == OWD2_TURN_5);
    // Play O at (0,2) to win.
    g.playMove(2);
    assert(g.isOver() == TicTacToeGame::kSecondPlayerWon);
    current_state = g.stateString();
    assert(current_state == OWD2_TURN_6);
    assert(g.leafEvalState() == -3);
    std::vector<TicTacToeMove> end_moves = {5, 7, 8};
    assert(g.availableMoves() == end_moves);
}

int main() {
    test_x_win_vertical();
    test_o_win_horizontal();
    test_x_win_diagonal1();
    test_o_win_diagonal2();
    std::cout << "Passed tic tac toe tests." << std::endl;
    return 0;
}
