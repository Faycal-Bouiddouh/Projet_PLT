//
// Created by tompi on 08/10/2026.
//ybyh

#include "Plate.h"
#include <iostream>
state::Plate::Plate(bool n, bool e, bool s, bool w): North(n), East(e), South(s), West(w) {

}

void state::Plate::turnPlate() {
    bool temp= North;
    North= West;
    West= South;
    South= East;
    East= temp;
    if (rotation ==3){
    rotation=0;}
    else {
        rotation+=1;
    }
};

int state::Plate::getRotation() const {
    return rotation;
}

void state::Plate::setRotation(int rotation) {
    this->rotation = rotation;
}

const std::string& state::Plate::getTreasure() const {
    return treasure;
}

void state::Plate::setTreasure(const std::string& treasure) {
    this->treasure = treasure;
}

bool state::Plate::getNorth() const {
    return North;
}

void state::Plate::setNorth(bool North) {
    this->North = North;
}

bool state::Plate::getSouth() const {
    return South;
}

void state::Plate::setSouth(bool South) {
    this->South = South;
}

bool state::Plate::getEast() const {
    return East;
}

void state::Plate::setEast(bool East) {
    this->East = East;
}

bool state::Plate::getWest() const {
    return West;
}

void state::Plate::setWest(bool West) {
    this->West = West;
}


void state::Plate::showPlate()
{
    const char* symbol = " ";

    if (North && South && East) {
        symbol = "├";
    }
    else if (North && South && West) {
        symbol = "┤";
    }
    else if (East && South && West) {
        symbol = "┬";
    }
    else if (North && East && West) {
        symbol = "┴";
    }
    else if (North && East) {
        symbol = "└";
    }
    else if (North && West) {
        symbol = "┘";
    }
    else if (South && East) {
        symbol = "┌";
    }
    else if (South && West) {
        symbol = "┐";
    }
    else if (North && South) {
        symbol = "|";
    }
    else if (East && West) {
        symbol = "─";
    }

    std::cout << symbol;
}
