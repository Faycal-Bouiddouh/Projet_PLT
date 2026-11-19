//
// Created by nicol on 11/11/2026.
//
#include "InsertPlateCOMMAND.h"

#include <iostream>
#include <ostream>
#include <state/Board.h>
#include <state/State.h>
using namespace render;


//InsertPlateCOMMAND::InsertPlateCOMMAND (state::State & state, int i, int j) : state(state), i(i), j(j) {}
InsertPlateCOMMAND::InsertPlateCOMMAND (state::Board & board, int i, int j) : board(board), i(i), j(j) {}
void InsertPlateCOMMAND::exec() {
    board.insertPlate(i,j);
}
