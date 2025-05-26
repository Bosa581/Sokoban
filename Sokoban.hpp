//  Copyright 2025 <Orbosa Igbinovia> [legal/copyright]
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>  //  for undo
#include <unordered_map>
#include <SFML/Graphics.hpp>

namespace SB {
enum class Direction {
    Up, Down, Left, Right
};

//  struct for undoing mvoes in the game
struct gameState {
    std::vector<std::vector<char>> board;
    sf::Vector2u playerPosition;
    std::vector<sf::Vector2u> boxPosition;
    unsigned int moveCount;
};

class Sokoban : public sf::Drawable {
 public:
    static const int TILE_SIZE = 64;

    Sokoban();
    explicit Sokoban(const std::string&);
    unsigned int height() const;
    unsigned int width() const;
    unsigned int getHeight() const;
    unsigned int getWidth() const;
    void setHeight(unsigned int height);
    void setWidth(unsigned int width);
    sf::Vector2u playerLoc() const;
    char getTile(unsigned int x, unsigned int y) const;
    std::vector<sf::Vector2u> getBoxes() const;
    bool isWon() const;
    void movePlayer(Direction dir);
    void reset();
    void undo();
    void redo();
    void loadTexture(char tile, const std::string& filename);
    friend std::istream& operator>>(std::istream& in, Sokoban& s);

 protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
    //  Additional feature
    sf::Clock gameClock;
    mutable sf::Time totalElapsedTime;
    mutable sf::Text timeText;
    std::unordered_map<char, sf::Texture> textures;
    std::unordered_map<char, sf::Sprite> sprites;
    unsigned int o_height;
    unsigned int o_width;
    std::string s;
    unsigned int moveCount = 0;
    sf::Font font;
    sf::Text moveText;
    std::string levelFilename;
    sf::Text winText;
    bool hasWon = false;
    std::stack<gameState> undoStack;

    std::vector<std::vector<char>> board;
    std::vector<std::vector<char>> originalBoard;
    sf::Vector2u playerPosition;
    std::vector<sf::Vector2u> boxPos;
    std::vector<sf::Vector2u> goalPosition;

    void loadBoardState(
        const std::vector<std::vector<char>>& newBoard,
        const sf::Vector2u& newPlayerPos,
        const std::vector<sf::Vector2u>& newBoxPositions,
        const std::vector<sf::Vector2u>& newGoalPositions);
    // Check if player can step on tile
    bool allowedTowalkOn(char tile) const;

    // Check if tile is a goal
    bool isGoalTile(unsigned int x, unsigned int y) const;
};

std::ostream& operator<<(std::ostream& out, const Sokoban& s);
std::istream& operator>>(std::istream& in, Sokoban& s);
}  // namespace SB
