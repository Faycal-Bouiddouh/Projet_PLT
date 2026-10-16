//
// Created by tompi on 08/10/2026.
//
#include "Player.h"

#include "State.h"

/**
 * Changes the position of the Player!
 * @param i /the line, supposed to be between 0 and 6.
 * @param j the column, supposed to be between 0 and 6/
 */
void state::Player::move(int i, int j) {
    position[0]=i;
    position[1]=j;
}

/**
 * Not implemented yet.
 * @param state
 */
void state::Player::obtainTreasure(State *state) {
    treasure_index+=1;
}

state::Player::Player(int i, int j) {
    position[0]=i;
    position[1]=j;
    starting_position[0]=i;
    starting_position[1]=j;
}