//  Copyright 2025 <Orbosa Igbinovia> [legal/copyright]
#include "Sokoban.hpp"
 int main(int argc, char* argv[] ) {
    if (argc != 2) {
        std::cerr << "Usage: ./Sokoban < levels.txt\n";
        return 1;
    }
    SB::Sokoban sokoban(argv[1]);  // reads the command line
    int ts = SB::Sokoban::TILE_SIZE;
    int windowWidth = sokoban.getWidth();
    int windowHeight = sokoban.getHeight();

    // 1o tiles going up and down times the each pixel being 64 bits
    sf::RenderWindow window
    (sf::VideoMode(windowWidth * ts, windowHeight * ts), "Sokoban!");
    //  while loop
    std::cout << sokoban << std::endl;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    sokoban.movePlayer(SB::Direction::Up);
                } else if (event.key.code == sf::Keyboard::Down) {
                    sokoban.movePlayer(SB::Direction::Down);
                } else if (event.key.code == sf::Keyboard::Left) {
                    sokoban.movePlayer(SB::Direction::Left);
                } else if (event.key.code == sf::Keyboard::Right) {
                    sokoban.movePlayer(SB::Direction::Right);
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code ==sf::Keyboard::R) {
                    sokoban.reset();
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::U) {
                    sokoban.undo();
                }
            }
        }
        window.clear();
        window.draw(sokoban);
        window.display();
    }
    return 0;
}
