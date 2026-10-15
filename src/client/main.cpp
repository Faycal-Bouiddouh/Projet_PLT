#include <iostream>

// The following lines are here to check that SFML is installed and working
#include <SFML/Graphics.hpp>
// LCOV_EXCL_START
void testSFML() {
    sf::Texture texture;
}
// end of test SFML

#include <state.h>

using namespace std;
using namespace state;

int main()
{
    Board board;
    board.createPlayground();
    board.showBoard();
    printf("\n");

    board.insertPlate(0,5);
    board.turnMasterPlate();
    board.insertPlate(0,5);
    board.showBoard();
    printf("\n");

    board.turnMasterPlate();
    board.insertPlate(6,3);
    board.showBoard();
    printf("\n");

    board.turnMasterPlate();
    board.insertPlate(2,0);
    board.showBoard();
    printf("\n");

    board.turnMasterPlate();
    board.insertPlate(3,6);
    board.showBoard();
    vector<array<int, 2>> atteignables = board.searchPath(1, 4);
    vector<array<int, 2>> atteignables2 = board.searchPath(2, 0);
    cout << "Cases atteignables depuis (1,4) : ";
    for (const auto& c : atteignables) {
        cout << "(" << c[0] << "," << c[1] << ") ";
    }
    cout << endl;
    cout << "Cases atteignables depuis (2,0) : ";
    for (const auto& c : atteignables2) {
        cout << "(" << c[0] << "," << c[1] << ") ";
    }
    cout << endl;
    cout << "It just works ! " << endl;

    return 0;
}

// LCOV_EXCL_END