#include <stdio.h>

#include "chess_game.h"

int main() {
    ChessGame g;
    bool o = g.isOver();
    printf("Value is %d\n", o);
    return 0;
}
