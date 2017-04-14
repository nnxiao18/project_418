#ifndef CHESS_H
#define CHESS_H

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <map>

using namespace std;

#define BOARD_SIZE 8
#define TYPES_OF_PIECES 6
#define NUM_PLAYERS 2
#define PAWN "pawn"
#define KNIGHT "knight"
#define BISHOP "bishop"
#define ROOK "rook"
#define QUEEN "queen"
#define KING "king"
typedef struct board_cell cell;
struct board_cell{
    string piece;
    int player; /* 0 is white; 1 is black; -1 is empty */
};

struct move{
    size_t from_row;
    size_t from_col;
    size_t to_row;
    size_t to_col;

    string piece;
    bool player;
};

//need to keep track of location, team, pieces
//typedef struct state_of_game *state;
class State{
    public:
    //std::vector<cell>* board = new std::vector<cell>[BOARD_SIZE];
    cell board[BOARD_SIZE][BOARD_SIZE];

    bool player; /* 0 is white; 1 is black */
    /* map of player to map of pieces to num left */
    std::map<string, int>* player0_pieces;
    std::map<string, int>* player1_pieces;

    struct move *last_move; // good for debugging
};

std::map<string, int> initPieces();
void initBoard(State S);
State initGame();
void printBoard(State S);

#endif
