#include <boost/test/unit_test.hpp>
#include "../../src/shared/state/Board.h"

using namespace state;

BOOST_AUTO_TEST_CASE(Test_Board_Initializes_All_Tiles)
{
    Board board;
    const auto& playground = board.getPlayground();

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            BOOST_CHECK_MESSAGE(
                playground[i][j] != nullptr,
                "Tuile manquante en (" << i << ", " << j << ")"
            );
        }
    }
}

BOOST_AUTO_TEST_CASE(Test_Board_Insertion_Positions)
{
    Board board;

    for (int i = 1; i <= 5; i += 2) {
        BOOST_CHECK(board.insertPlate(0, i));  // Bord du haut
        BOOST_CHECK(board.insertPlate(i, 0));  // Bord de gauche
        BOOST_CHECK(board.insertPlate(6, i));  // Bord du bas
        BOOST_CHECK(board.insertPlate(i, 6));  // Bord de droite
    }

    BOOST_CHECK(!board.insertPlate(0, 0));   // Coin
    BOOST_CHECK(!board.insertPlate(0, 2));   // Indice pair sur le bord du haut
    BOOST_CHECK(!board.insertPlate(3, 3));   // Centre du plateau
    BOOST_CHECK(!board.insertPlate(-1, 3));  // Ligne hors de la grille
    BOOST_CHECK(!board.insertPlate(3, 7));   // Colonne hors de la grille
}


