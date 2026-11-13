//
// Created by tompi on 16/10/2026.
//

#include "Game.h"
#include "ListCommands.h"
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

using namespace std;
using namespace state;

client::Game::Game() {
}

void client::Game::run() {
    Board &board = state.getBoard();
    board.createPlayground();
    std::array<Player, 2> &players = state.getPlayers();
    string s;
    std::array<int, 2> mov{};

    // Deal twelve different objectives to the active player
    array<Treasure, 24> treasures{};
    for (int k = 0; k < static_cast<int>(treasures.size()); ++k) {
        treasures[k] = static_cast<Treasure>(k + 1);
    }

    mt19937 generator(random_device{}());
    shuffle(treasures.begin(), treasures.end(), generator);

    array<Treasure, 12> objectives{};
    copy_n(treasures.begin(), objectives.size(), objectives.begin());
    players[0].setTreasureList(objectives);

    const array<int, 2> startingPosition = players[0].getPosition();
    int obtainedTreasures = 0;

    auto treasureName = [](Treasure treasure) {
        const array<const char*, 25> names = {
            "Aucun", "Dragon", "Fantome", "Genie", "Sorciere",
            "Princesse", "Chauve-souris", "Chouette", "Rat",
            "Araignee", "Scarabee", "Lezard", "Papillon", "Livre",
            "Bourse", "Coffre", "Couronne", "Cle", "Anneau",
            "Chandelier", "Carte", "Epee", "Casque", "Crane", "Gemme"
        };
        return names[static_cast<int>(treasure)];
    };

    // Collect only the active player's position
    auto getPositions = [&players]() {
        vector<array<int, 2>> positions;
        positions.push_back(players[0].getPosition());
        return positions;
    };

    // Check the format before reading characters by index
    auto readPosition = [](const string &text, array<int, 2> &position) {
        if (text.size() != 5 || text[0] != '(' || text[2] != ',' ||
            text[4] != ')' || text[1] < '0' || text[1] > '6' ||
            text[3] < '0' || text[3] > '6') {
            return false;
        }

        position = {text[1] - '0', text[3] - '0'};
        return true;
    };

    cout << "Trouve tes 12 tresors dans l'ordre, puis reviens en ("
         << startingPosition[0] << "," << startingPosition[1] << ").\n"
         << "Coordonnees : (ligne,colonne), de 0 a 6, sans espaces.\n";

    while (true) {
        Treasure currentTreasure = players[0].getCurrentTreasure();
        board.showBoard({}, getPositions(), currentTreasure);

        cout << "Tresors obtenus : " << obtainedTreasures << "/12\n";
        if (currentTreasure != Treasure::NONE) {
            cout << "Objectif : \033[1;35m"
                 << treasureName(currentTreasure) << " ["
                 << (static_cast<int>(currentTreasure) < 10 ? "0" : "")
                 << static_cast<int>(currentTreasure) << "]\033[0m\n";
        } else {
            cout << "Tous les tresors sont obtenus ! Retourne en ("
                 << startingPosition[0] << "," << startingPosition[1] << ").\n";
        }

        cout << "Inserer ou ? (ligne,colonne) : ";
        if (!(cin >> s)) {
            return;
        }
        if (!readPosition(s, mov)) {
            cout << "Format attendu : (ligne,colonne), de 0 a 6.\n";
            continue;
        }

        // Update tiles and players together
        if (!state.applyInsertion(mov[0], mov[1])) {
            cout << "Insertion interdite.\n";
            continue;
        }

        // Read the position after the insertion
        std::array<int, 2> position = players[0].getPosition();
        vector<array<int, 2>> atteignables =
            board.searchPath(position[0], position[1]);
        board.showBoard(atteignables, getPositions(), currentTreasure);

        cout << "Cases atteignables depuis ("
             << position[0] << "," << position[1] << ") :\n";
        for (const auto &c : atteignables) {
            cout << "(" << c[0] << "," << c[1] << ") ";
        }
        cout << '\n';

        // Retry the move without inserting another tile
        while (true) {
            cout << "Aller ou ? (ligne,colonne) : ";
            if (!(cin >> s)) {
                return;
            }
            if (!readPosition(s, mov)) {
                cout << "Format attendu : (ligne,colonne), de 0 a 6.\n";
                continue;
            }
            if (!state.verifyMovementValidity(mov[0], mov[1], atteignables)) {
                cout << "Case inaccessible. Choisis une case atteignable.\n";
                continue;
            }

            players[0].move(mov[0], mov[1]);
            break;
        }

        // State validates the objective at the final position
        if (state.obtainTreasure(0)) {
            ++obtainedTreasures;
            cout << "\033[1;35mTresor obtenu : "
                 << treasureName(currentTreasure) << " !\033[0m\n";
        }

        if (players[0].getCurrentTreasure() == Treasure::NONE &&
            players[0].getPosition() == startingPosition) {
            board.showBoard({}, getPositions(), Treasure::NONE);
            cout << "Victoire ! Les 12 tresors sont obtenus et tu es revenu au depart.\n";
            break;
        }

        cout << endl;
    }
}

