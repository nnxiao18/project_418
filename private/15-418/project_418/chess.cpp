#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include "chess.h"

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
/*
typedef struct board_cell cell;
struct board_cell{
    string piece;
    int player; // 0 is white; 1 is black; -1 is empty
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

    bool player; // 0 is white; 1 is black
    // map of player to map of pieces to num left
    std::map<string, int> player0_pieces;
    std::map<string, int> player1_pieces;

    struct move *last_move; // good for debugging
};

void initBoard(State S);
std::map<string,int> initPieces();
*/
/* given a state, outputs all possible moves: array of states in which the possible move was made */
void possMoves(State S){
    //all_next_moves;
    return;
}

void makeMove(State S){
    return;
}

/* ------------------ functions to play chess ----------------- */
State initGame(){
    State start;// = (state) malloc(sizeof(state) * 1);
//    cell b[BOARD_SIZE][BOARD_SIZE];
 //   cell (*b)[BOARD_SIZE] = new cell[BOARD_SIZE][BOARD_SIZE];
    /*
    for (size_t i = 0; i < BOARD_SIZE; i++){
        b[i] = new cell[BOARD_SIZE];
    }
    */
//    start.board = cell[BOARD_SIZE][BOARD_SIZE];//b;
    initBoard(start);
    start.player = 0;
    start.player0_pieces = initPieces();
    start.player1_pieces = initPieces();
    return start;
}

std::map<string,int> initPieces(){
    std::map<string,int> num_pieces =
        new std::map<string,int>[TYPES_OF_PIECES];
    num_pieces[KING] = 1;
    num_pieces[QUEEN] = 1;
    num_pieces[ROOK] = 2;
    num_pieces[BISHOP] = 2;
    num_pieces[KNIGHT] = 2;
    num_pieces[PAWN] = 8;
    return num_pieces;
}

void initBoard(State S){
    for (size_t i = 0; i < BOARD_SIZE; i++){
        for (size_t j = 0; j < BOARD_SIZE; j++){
            if (i == 0 or i == 1){ //black pieces
                S.board[i][j].player = 1;
                if (i == 1){
                    S.board[i][j].piece = PAWN; //black pawns
                }
            } if (i == BOARD_SIZE - 1 or i == BOARD_SIZE - 2){ //white pieces
                S.board[i][j].player = 0;
                if (i == BOARD_SIZE - 2){
                    S.board[i][j].piece = PAWN; //white pawns
                }
            } if (i == 0 or i == BOARD_SIZE -1){ //first and last row
                if (j == 0 or j == BOARD_SIZE - 1){ //rooks
                    S.board[i][j].piece = ROOK;
                } else if (j == 1 or j == BOARD_SIZE - 2){ //knights
                    S.board[i][j].piece = KNIGHT;
                } else if (j == 2 or j == BOARD_SIZE - 3){
                    S.board[i][j].piece = BISHOP;
                } else if (j == 3){
                    S.board[i][j].piece = QUEEN;
                } else {
                    S.board[i][j].piece = KING;
                }

            } else { //empty rows
                S.board[i][j].player = -1;
                S.board[i][j].piece = "";
            }
        }
    }
    return;
}

void randomBoard(){
    return;
}

bool gameOver(State S){
    if (S.player1_pieces[KING] == 0 or S.player0_pieces[KING] == 0){
        return true;
    }
    /* if no more moves for that player */
    return false; //(possMoves(S).size() == 0);
}

/* ------------------ For debugging purposes ------------------ */
void printBoard(State S){
    for (size_t i = 0; i < BOARD_SIZE; i++){
        for (size_t j = 0; j < BOARD_SIZE; j++){
            if (j == 0){
                printf("\n");
            } if (S.board[i][j].player == -1){
                printf("----, ");
            } else {
                printf("player: %d; %s", S.board[i][j].player,
                        (S.board[i][j].piece).c_str());
            }
        }
    }
    return;
}

void printNumPieces(State S){
    printf("pieces of player0:\n");
    printf("kings: %d, ",S.player0_pieces[KING]);
    printf("queens: %d, ",S.player0_pieces[QUEEN]);
    printf("rooks: %d, ",S.player0_pieces[ROOK]);
    printf("knights: %d, ",S.player0_pieces[KNIGHT]);
    printf("bishops: %d, ",S.player0_pieces[BISHOP]);
    printf("pawns: %d, ",S.player0_pieces[PAWN]);

    printf("\npieces of player1:\n");
    printf("kings: %d, ",S.player1_pieces[KING]);
    printf("queens: %d, ",S.player1_pieces[QUEEN]);
    printf("rooks: %d, ",S.player1_pieces[ROOK]);
    printf("knights: %d, ",S.player1_pieces[KNIGHT]);
    printf("bishops: %d, ",S.player1_pieces[BISHOP]);
    printf("pawns: %d, ",S.player1_pieces[PAWN]);


}

/* how init board looks like */
// row 0: black rook, knight, bishop, queen, king, bishop, knight, rook
// row 1: black pawns
// row 2-5: empty
// row 6: white pawns
// row 7: white rook, knight, bishop, queen, king, bishop, knight, rook
