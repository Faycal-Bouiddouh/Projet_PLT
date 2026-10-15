//
// Created by tompi on 08/10/2026.
//

#include "Board.h"
#include "L.h"
#include "T.h"
#include "I.h"

#include <iostream>
#include <memory>
#include <queue>
#include <array>

state::Board::Board(): playground{}, masterplate{} {
}

void state::Board::insertPlate(int i, int j) {
    auto tempPlate = std::move(masterplate);
    switch (i) {
        case 0:
            masterplate = std::move(playground[6][j]);
            for (int l = 6; l > 0; --l) {
                playground[l][j] = std::move(playground[l - 1][j]);
            }
            playground[0][j] = std::move(tempPlate);
            break;

        case 6:
            masterplate = std::move(playground[0][j]);
            for (int l = 0;  l<6 ; l++) {
                playground[l][j] = std::move(playground[l + 1][j]);
            }
            playground[6][j]=std::move(tempPlate);
            break;
    }

    switch (j) {
        case 0:
            masterplate = std::move(playground[i][6]);
            for (int l = 6;  l>0 ; l--) {
                playground[i][l] = std::move(playground[i][l-1]);
            }
            playground[i][0]=std::move(tempPlate);
            break;

        case 6:
            masterplate = std::move(playground[i][0]);
            for (int l = 0;  l<6 ; l++) {
                playground[i][l] = std::move(playground[i][l+1]);
            }
            playground[i][6]=std::move(tempPlate);
            break;
    }
}


void state::Board::turnMasterPlate(){
    masterplate->turnPlate();
}

void state::Board::showBoard() {
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            playground[i][j]->showPlate();
        }
        std::cout << '\n';
    }
}

void state::Board::createPlayground() {
    for (auto& row : playground) {
        for (auto& plate : row) {
            plate = std::make_unique<state::L>();
        }
    }
    masterplate = std::make_unique<state::T>();
}

// Returns every cell reachable from (i, j), start cell included.
// Cells are ordered from the nearest to the farthest (BFS order).
// i = row, j = column. Returns an empty vector if the start is invalid.
std::vector<std::array<int, 2>> state::Board::searchPath (int i, int j)
{
    std::vector<std::array<int, 2>> reachable;

    // Start is outside the board: nothing to explore
    if (i < 0 || i >= 7 || j < 0 || j >= 7) return reachable;

    // No plate on the start cell: nothing to explore
    if (!playground[i][j]) return reachable;

    // Directions: 0 = North, 1 = East, 2 = South, 3 = West
    // The opposite of dir is (dir + 2) % 4
    const int di[4] = { -1, 0, 1, 0 };
    const int dj[4] = { 0, 1, 0, -1 };

    // Does the plate at (a, b) have an opening in direction dir?
    // A cell without a plate counts as closed.
    auto isOpen = [this](int a, int b, int dir) {
        const std::unique_ptr<Plate>& plate = playground[a][b];
        if (!plate) return false;
        switch (dir) {
        case 0:  return plate->getNorth();
        case 1:  return plate->getEast();
        case 2:  return plate->getSouth();
        case 3:  return plate->getWest();
        default: return false;
        }
    };

    bool visited[7][7] = {};               // cells already discovered
    std::queue<std::array<int, 2>> file;   // discovered cells waiting to be explored

    // Start from (i, j)
    visited[i][j] = true;
    file.push({ i, j });

    while (!file.empty()) {
        // Take the oldest cell in the queue
        std::array<int, 2> courante = file.front();
        file.pop();
        reachable.push_back(courante);     // it is reachable

        // Look at the 4 neighbours
        for (int dir = 0; dir < 4; ++dir) {
            int ni = courante[0] + di[dir];
            int nj = courante[1] + dj[dir];

            if (ni < 0 || ni >= 7 || nj < 0 || nj >= 7) continue;   // off the board
            if (visited[ni][nj]) continue;                          // already seen

            // Passage exists only if both plates open toward each other
            if (isOpen(courante[0], courante[1], dir) &&
                isOpen(ni, nj, (dir + 2) % 4)) {
                visited[ni][nj] = true;    // mark when discovered, not when popped
                file.push({ ni, nj });
            }
        }
    }
    return reachable;
}