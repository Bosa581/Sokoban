#Sokoban

## Contact

Name: Orobosa Igbinovia    
Orobosa581@gmail.com

## Description

This project implements a tile-based game called sokoban. The game loads a level from a file and draws the game board to the screen using textures. Each type of tile — wall, box, storage location, player, and floor — is represented by a sprite.

### Features

- The game reads the level size and tile layout from a text file.
- Each tile type is rendered using a corresponding image sprite.
- The game board is displayed using SFML’s `sf::Drawable` interface by overriding the `draw()` function.
- Textures are loaded once and reused across sprites for efficiency.
- A move counter is displayed on screen (**extra credit**).
- A gameplay timer is displayed to show the number of seconds played.

#### Part A

- Implemented `Sokoban` class in the `SB` namespace.
- Implemented the `>>` operator to read level files with a height, width, and tile data.
- Created and managed textures for each tile type (`#`, `.`, `@`, `A`, `a`).
- Implemented drawing logic using `sf::Sprite` and `sf::RenderTarget`.
- Dynamically sized the window based on the level dimensions.
- Verified that levels of different sizes render correctly.

#### Part B

- Implemented movement logic via `movePlayer(Direction)`, supporting `Up`, `Down`, `Left`, and `Right`.
- Prevented invalid movements into walls, offscreen, or when boxes cannot be pushed.
- Implemented box-pushing mechanics where boxes are only moved if the space beyond is walkable.
- Added a win detection system using two symmetric conditions:
  - All boxes are on storage spaces
  - All storage spaces are covered by boxes
- Pressing `R` resets the game to its original loaded state.
- Pressing `U` undoes the last move, and undo history is tracked using a a stack, gamestate
- `isWon()` disables movement and displays a congratulatory message when the win condition is met.
- Textures are updated per-direction, allowing the player sprite to visually face its movement direction.
- Implemented a gameplay timer using sfml's CLock, which displays real-time seconds played until the puzzle is solved. Once solved, the timer freezes.
- Used STL algorithms like the `std::find` and `std::all_of` for win condition logic.

### Memory

The level data is stored as a 2D `std::vector<std::vector<char>>` matrix, where each inner vector represents a row of the board. Box and goal positions are stored as `std::vector<sf::Vector2u>`. Undo history is maintained using a `std::stack` of custom `gameState` structs. Texture memory is reused through a `std::unordered_map<char, sf::Texture>`.

### Lambdas

No `<algorithm>` functions or lambda expressions were required for Part A.

### Issues

- I Still have issues passing one test case** — 1.9 (Missing Symbol) and 4.5 (swapoff logic) from part B
- Initially had issues where sprites were not showing up — this was because textures were not loaded before assigning them to sprites.
- The `draw()` method initially failed until debug output was added for unknown tile types.
- The move counter was implemented, but the font failed to load until `/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf` was added and pathing was fixed.
- Integrating the gameplay timer required using `mutable` to allow updating time from `const` methods.

### Extra Credit

- Implemented a move counter that tracks the number of valid player moves and renders it live on screen
- Implemented a real-time gameplay timer using `sf::Clock` to show how long the player has been playing,
    when the game ends the timer stops and when you resetart, the timer does too
- Implemented an Undo method that can undo every move that has been made.
- The player changes direction when moving

## Acknowledgements

- Kenney Sokoban Pack
- SFML Library
