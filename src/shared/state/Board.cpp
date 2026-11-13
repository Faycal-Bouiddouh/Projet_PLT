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
#include <algorithm>
#include <utility>
#include <random>
#include <vector>

state::Board::Board(): playground{}, masterplate{} {

}


bool state::Board::verifyInsertValidity(int i, int j) {
     int legal_inserts[12][2] = {
        {0, 1}, {0, 3}, {0, 5},
        {6, 1}, {6, 3}, {6, 5},
        {1, 0}, {3, 0}, {5, 0},
        {1, 6}, {3, 6}, {5, 6}
    };

    for ( auto& pos : legal_inserts) {
        if (pos[0] == i && pos[1] == j) {
            return true;
        }
    }
    return false;
}

bool state::Board::insertPlate(int i, int j) {
    if (verifyInsertValidity(i,j)) {
        auto tempPlate = std::move(masterplate);
        switch (i) {
            case 0:
                masterplate = std::move(playground[6][j]);
                for (int l = 6; l > 0; --l) {
                    playground[l][j] = std::move(playground[l - 1][j]);
                }
                playground[0][j] = std::move(tempPlate);
                return true;

            case 6:
                masterplate = std::move(playground[0][j]);
                for (int l = 0;  l<6 ; l++) {
                    playground[l][j] = std::move(playground[l + 1][j]);
                }
                playground[6][j]=std::move(tempPlate);
                return true;
        }

        switch (j) {
            case 0:
                masterplate = std::move(playground[i][6]);
                for (int l = 6;  l>0 ; l--) {
                    playground[i][l] = std::move(playground[i][l-1]);
                }
                playground[i][0]=std::move(tempPlate);
                return true;

            case 6:
                masterplate = std::move(playground[i][0]);
                for (int l = 0;  l<6 ; l++) {
                    playground[i][l] = std::move(playground[i][l+1]);
                }
                playground[i][6]=std::move(tempPlate);
                return true;
        }
    }
    return false;
}


void state::Board::turnMasterPlate(){
    masterplate->turnPlate();
}

void state::Board::showBoard(
    const std::vector<std::array<int, 2>> &reachable,
    const std::vector<std::array<int, 2>> &playerPositions,
    Treasure expected)
{
    const char *reset = "\033[0m";
    const char *reachableColor = "\033[1;33m";
    const char *treasureColor = "\033[1;30;105m";
    const std::array<const char*, 4> playerColors = {
        "\033[1;37;42m", "\033[1;37;41m",
        "\033[1;37;44m", "\033[1;37;46m"
    };

    auto drawTile = [reset, treasureColor](
        Plate *plate, int id, const char *color)
    {
        std::cout << color;
        if (plate) {
            plate->showPlate();
        } else {
            std::cout << "?";
        }

        if (id != 0) {
            std::cout << treasureColor
                      << (id < 10 ? "0" : "") << id;
        } else {
            std::cout << "  ";
        }
        std::cout << reset;
    };

    std::cout << "\n     ";
    for (int j = 0; j < 7; ++j) {
        std::cout << j << "   ";
    }
    std::cout << '\n';

    for (int i = 0; i < 7; ++i) {
        std::cout << " " << i << "  ";
        for (int j = 0; j < 7; ++j) {
            const std::array<int, 2> position = {i, j};
            Plate *plate = playground[i][j].get();
            bool isReachable =
                std::find(reachable.begin(), reachable.end(), position) !=
                reachable.end();

            int id = 0;
            const char *color = isReachable ? reachableColor : reset;

            // Display only the current objective
            if (expected != Treasure::NONE &&
                plate && plate->hasTreasure(expected)) {
                id = static_cast<int>(expected);
                color = treasureColor;
            }

            /*
            // Uncomment this block to display every treasure
            for (int value = 1;
                 value <= static_cast<int>(Treasure::GEM); ++value) {
                if (plate && plate->hasTreasure(
                                 static_cast<Treasure>(value))) {
                    id = value;
                    color = treasureColor;
                    break;
                }
            }
            */

            // Keep the player visible; the objective number stays pink
            for (std::size_t k = 0;
                 k < playerPositions.size() && k < playerColors.size(); ++k) {
                if (playerPositions[k] == position) {
                    color = playerColors[k];
                    break;
                }
            }

            drawTile(plate, id, color);
            std::cout << ' ';
        }
        std::cout << '\n';
    }

    if (expected != Treasure::NONE) {
        std::cout << treasureColor << " Tresor recherche " << reset << "  ";
    }
    std::cout << reachableColor << " Case atteignable " << reset << '\n';

    for (std::size_t k = 0;
         k < playerPositions.size() && k < playerColors.size(); ++k) {
        std::cout << playerColors[k] << " Joueur " << k + 1 << " "
                  << reset << "  ";
    }
    std::cout << '\n';
}

void state::Board::createPlayground() {
    // Create the board tiles
    for (auto& row : playground) {
        for (auto& plate : row) {
            plate = std::make_unique<state::L>();
        }
    }

    masterplate = std::make_unique<state::T>();

    // Collect positions outside the four starting corners
    std::vector<std::array<int, 2>> treasurePositions;

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            playground[i][j]->setTreasure(state::Treasure::NONE);

            bool isCorner =
                (i == 0 || i == 6) && (j == 0 || j == 6);

            if (!isCorner) {
                treasurePositions.push_back({i, j});
            }
        }
    }

    masterplate->setTreasure(state::Treasure::NONE);

    // Shuffle the available positions
    std::mt19937 generator(std::random_device{}());

    std::shuffle(
        treasurePositions.begin(),
        treasurePositions.end(),
        generator
    );

    // Place each of the twenty-four treasures once
    for (int id = 1;
         id <= static_cast<int>(state::Treasure::GEM);
         ++id) {
        const auto& position = treasurePositions[id - 1];

        playground[position[0]][position[1]]->setTreasure(
            static_cast<state::Treasure>(id)
        );
    }
}

std::vector<std::array<int, 2>> state::Board::searchPath (int i, int j)
{
    std::vector<std::array<int, 2>> reachable;
 
    if (i < 0 || i >= 7 || j < 0 || j >= 7) return reachable;
 
    if (!playground[i][j]) return reachable;
 
    const int di[4] = { -1, 0, 1, 0 };
    const int dj[4] = { 0, 1, 0, -1 };
 
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
 
    bool visited[7][7] = {};               
    std::queue<std::array<int, 2>> file;   
 
    visited[i][j] = true;
    file.push({ i, j });
 
    while (!file.empty()) {
        std::array<int, 2> courante = file.front();   
        file.pop();                                   
        reachable.push_back(courante);                
 
        for (int dir = 0; dir < 4; ++dir) {           
            int ni = courante[0] + di[dir];
            int nj = courante[1] + dj[dir];
 
            if (ni < 0 || ni >= 7 || nj < 0 || nj >= 7) continue;   
            if (visited[ni][nj]) continue;                          
 
            if (isOpen(courante[0], courante[1], dir) &&
                isOpen(ni, nj, (dir + 2) % 4)) {
                visited[ni][nj] = true;     
                file.push({ ni, nj });
            }
        }
    }
    return reachable;
}

bool state::Board::hasTreasureAt(
    int i, int j, state::Treasure expected) const
{
    if (i < 0 || i >= 7 || j < 0 || j >= 7) {
        return false;
    }

    if (!playground[i][j]) {
        return false;
    }

    return playground[i][j]->hasTreasure(expected);
}