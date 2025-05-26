
//  Copyright 2025 <Orbosa Igbinovia> [legal/copyright]
#include "Sokoban.hpp"
#include <fstream>
#include <stdexcept>

namespace SB {

// load textures from file and into the sprite.
void Sokoban::loadTexture(char tile, const std::string& filename) {
    if (!textures[tile].loadFromFile(filename)) {
        std::cerr << "Failed to load texture for tile '" << tile
                  << "' from file: " << filename << std::endl;
    } else {
        sprites[tile].setTexture(textures[tile]);
    }
}

// Default constructor
Sokoban::Sokoban() : o_height(0), o_width(0) {
    moveCount = 0;
    if (font.getInfo().family.empty()) {
        if (!font.loadFromFile
            ("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            std::cerr << "Warning: Could not load system font."
             "Move counter won't be displayed.\n";
            return;
        }
    }
    // Load textures for each character type
    loadTexture('#', "block_06.png");
    loadTexture('.', "ground_01.png");
    loadTexture('A', "crate_03.png");
    loadTexture('a', "ground_04.png");
    loadTexture('@', "player_05.png");

    for (const auto& [tile, texture] : textures) {
        sprites[tile].setTexture(texture);
    }
}

// Constructor to load the level from a file
Sokoban::Sokoban(const std::string& filename) {
    moveCount = 0;
    levelFilename = filename;

    if (font.getInfo().family.empty()) {
        if (!font.loadFromFile
            ("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            std::cerr << "Warning: Could not load system font."
             "Move counter won't be displayed.\n";
            return;
        }
    }
    moveText.setFont(font);
    moveText.setCharacterSize(18);
    moveText.setFillColor(sf::Color::Yellow);
    moveText.setPosition(10.f, 10.f);
    moveText.setString("Moves: 0");

    winText.setCharacterSize(32);
    winText.setFont(font);
    winText.setFillColor(sf::Color::Blue);
    winText.setString("CONGRATS, YOU WIN.");
    winText.setPosition(150.f, 300.f);

    timeText.setFont(font);
    timeText.setCharacterSize(18);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(10.f, 35.f);
    timeText.setString("Time: 0s");

    loadTexture('#', "block_06.png");
    loadTexture('.', "ground_01.png");
    loadTexture('A', "crate_03.png");
    loadTexture('a', "ground_04.png");
    loadTexture('@', "player_05.png");

    for (const auto& [tile, texture] : textures) {
        sprites[tile].setTexture(texture);
    }

    std::ifstream file(filename);
    if (file) {
        file >> *this;
        originalBoard = board;  // for my reset method
    } else {
        throw std::runtime_error("Unable to open file: " + filename);
    }
}

unsigned int Sokoban::height() const { return o_height; }
unsigned int Sokoban::width() const { return o_width; }
unsigned int Sokoban::getHeight() const { return o_height; }
unsigned int Sokoban::getWidth() const { return o_width; }
void Sokoban::setHeight(unsigned int height) { o_height = height; }
void Sokoban::setWidth(unsigned int width) { o_width = width; }
sf::Vector2u Sokoban::playerLoc() const { return playerPosition; }

char Sokoban::getTile(unsigned int x, unsigned int y) const {
    if (y < board.size() && x < board[y].size()) {
        return board[y][x];
    }
    return '?';  // invalid tile
}

std::vector<sf::Vector2u> Sokoban::getBoxes() const {
    return boxPos;
}

bool Sokoban::isWon() const {
    // Case 1: Every goal has a box
    bool allGoalsCovered = std::all_of(goalPosition.begin(), goalPosition.end(),
        [this](const sf::Vector2u& goal) {
            return std::find(boxPos.begin(),
             boxPos.end(), goal) != boxPos.end();
        });

    // Case 2: Every box is on a goal
    bool allBoxesPlaced = std::all_of(boxPos.begin(), boxPos.end(),
        [this](const sf::Vector2u& box) {
            return std::find(goalPosition.begin(),
             goalPosition.end(), box) != goalPosition.end();
        });

    if (allGoalsCovered || allBoxesPlaced) {
        totalElapsedTime = gameClock.getElapsedTime();
        std::cout << "Congrats, You've won!!" << std::endl;
        return true;
    }

    return false;
}



void Sokoban::movePlayer(Direction dir) {
    int dx = 0, dy = 0;
    switch (dir) {
        case Direction::Up:    dy = -1;
        loadTexture('@', "player_08.png"); break;
        case Direction::Down:  dy = 1;
        loadTexture('@', "player_05.png"); break;
        case Direction::Left:  dx = -1;
        loadTexture('@', "player_20.png"); break;
        case Direction::Right: dx = 1;
        loadTexture('@', "player_17.png"); break;
    }

    undoStack.push(gameState{board, playerPosition, boxPos, moveCount});

    unsigned int px = playerPosition.x, py = playerPosition.y;
    unsigned int nx = px + dx, ny = py + dy;

    if (nx >= 0 && nx < o_width
     && ny >= 0 && ny < o_height) {
        char nextTile = board[ny][nx];

        if (nextTile == '.' || nextTile == 'a') {
            if (std::find(goalPosition.begin(), goalPosition.end(),
             sf::Vector2u{px, py}) != goalPosition.end()) {
                // Restore goal under player if player was on it
                board[py][px] = 'a';
            } else {
                board[py][px] = '.';  // Otherwise, just clear the tile
            }
            board[ny][nx] = '@';  // Move player to the new tile
            playerPosition =
             {static_cast<unsigned>(nx), static_cast<unsigned>(ny)};
            moveCount++;
        } else if (nextTile == 'A') {
            int bx = nx + dx, by = ny + dy;
            if (bx >= 0 && bx < static_cast<int>(o_width)
             && by >= 0 && by < static_cast<int>(o_height)) {
                char beyondBox = board[by][bx];
                if (beyondBox == '.' || beyondBox == 'a') {
                    // Move box onto storage or floor
                    if (std::find(goalPosition.begin(),
                     goalPosition.end(), sf::Vector2u{nx, ny})
                      != goalPosition.end()) {
                        board[ny][nx] = 'a';  // Restore goal under the box
                    } else {
                        board[ny][nx] = '.';  // Just floor under the box
                    }

                    // Move the box
                    board[by][bx] = 'A';
                    // Move player
                    board[ny][nx] = '@';

        // Restore the player’s previous position if it was on a goal
                    if (std::find(goalPosition.begin(),
                     goalPosition.end(), sf::Vector2u{px, py})
                      != goalPosition.end()) {
                        board[py][px] = 'a';  // Restore goal under player
                    } else {
                        board[py][px] = '.';  // Otherwise, just floor
                    }

                    // Update box position in the list
                    for (auto& box : boxPos) {
                        if (box.x == static_cast<unsigned>(nx) &&
                            box.y == static_cast<unsigned>(ny)) {
                            box.x = static_cast<unsigned>(bx);
                            box.y = static_cast<unsigned>(by);
                            break;
                        }
                    }

                    playerPosition =
                     {static_cast<unsigned>(nx), static_cast<unsigned>(ny)};
                    moveCount++;
                }
            }
        }
        moveText.setString("Moves: " + std::to_string(moveCount));
    }
    if (isWon()) {
        hasWon = true;
    }
}


void Sokoban::reset() {
    board = originalBoard;
    boxPos.clear();
    moveCount = 0;
    moveText.setString("Moves: 0");
    timeText.setString("Time: 0s");
    gameClock.restart();

    for (unsigned int y = 0; y < board.size(); y++) {
        for (unsigned int x = 0; x < board[y].size(); x++) {
            if (board[y][x] == '@') {
                playerPosition = {x, y};
            } else if (board[y][x] == 'A') {
                boxPos.push_back({x, y});
            }
        }
    }
}

void Sokoban::undo() {
    if (!undoStack.empty()) {
        gameState prev = undoStack.top();
        undoStack.pop();
        board = prev.board;
        playerPosition = prev.playerPosition;
        boxPos = prev.boxPosition;
        moveCount = prev.moveCount;
        moveText.setString("Moves: " + std::to_string(moveCount));
    }
}

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (unsigned int y = 0; y < o_height; ++y) {
        for (unsigned int x = 0; x < o_width; ++x) {
            char tile = board[y][x];
            if (sprites.find(tile) != sprites.end()) {
                sf::Sprite sprite = sprites.at(tile);
                sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                target.draw(sprite, states);
            } else {
                std::cerr << "No sprite found for tile: " << tile << std::endl;
            }
        }
    }
    target.draw(moveText, states);
    if (hasWon) {
        target.draw(winText, states);
    }
    sf::Time displayTime =
     hasWon ? totalElapsedTime : gameClock.getElapsedTime();
    timeText.setString
    ("Time: " + std::to_string(static_cast<int>
        (displayTime.asSeconds())) + "s");
    target.draw(timeText, states);
}

std::ostream& operator<<(std::ostream& out, const Sokoban& s) {
    out << s.height() << " " << s.width() << std::endl;
    out << "#" << " " << "." << " " << "A"
     << " " << "a" << " " << "@" << std::endl;
    for (unsigned int y = 0; y < s.getHeight(); y++) {
        for (unsigned int x = 0; x < s.getWidth(); x++) {
            out << s.getTile(x, y);
        }
        out << std::endl;
    }
    return out;
}

std::istream& operator>>(std::istream& in, Sokoban& s) {
    unsigned int width, height;
    in >> height >> width;
    s.setHeight(height);
    s.setWidth(width);

    std::vector<std::vector<char>> tempBoard(height, std::vector<char>(width));
    std::vector<sf::Vector2u> tempBoxes;
    sf::Vector2u tempPlayer;
    std::vector<sf::Vector2u> tempGoals;

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            in >> tempBoard[y][x];
            if (tempBoard[y][x] == '@') {
                tempPlayer = {x, y};
            } else if (tempBoard[y][x] == 'A') {
                tempBoxes.push_back({x, y});
            } else if (tempBoard[y][x] == 'a') {
                tempGoals.push_back({x, y});
            }
        }
    }

    s.loadBoardState(tempBoard, tempPlayer, tempBoxes, tempGoals);

    return in;
}

void Sokoban::loadBoardState(
const std::vector<std::vector<char>>& newBoard,
const sf::Vector2u& newPlayerPos,
const std::vector<sf::Vector2u>& newBoxPositions,
const std::vector<sf::Vector2u>& newGoalPositions
) {
    board = newBoard;
    playerPosition = newPlayerPos;
    boxPos = newBoxPositions;
    goalPosition = newGoalPositions;
}

bool Sokoban::allowedTowalkOn(char tile) const {
    return tile != '#' && tile != 'A';
}

bool Sokoban::isGoalTile(unsigned int x, unsigned int y) const {
    return std::find(goalPosition.begin(),
    goalPosition.end(), sf::Vector2u{x, y}) != goalPosition.end();
}

}  // namespace SB
