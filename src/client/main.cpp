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
    Player joueur;
    board.createPlayground();

    vector<array<int, 2>> atteignables = board.searchPath(1, 4);
    cout << "Cases atteignables depuis (1,4) : ";
    for (const auto& c : atteignables) {
        cout << "(" << c[0] << "," << c[1] << ") ";
    }
    cout << endl;
    

    cout << "It just works ! " << endl;

    return 0;
}

// LCOV_EXCL_END