//  Copyright 2025 <Orbosa Igbinovia> [legal/copyright]
#include <iostream>
#include <string>
#include "Sokoban.hpp"
#define BOOST_TEST_MODULE Main
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(Boxisntawalkthrough) {
    SB::Sokoban s("level1.lvl");

    // Move to the right three times to push the box at (6,6)
    s.movePlayer(SB::Direction::Right);  // from (4,6) to (5,6)
    s.movePlayer(SB::Direction::Right);  // to (6,6), adjacent to box
    s.movePlayer(SB::Direction::Right);  // push box from (6,6) to (7,6)

    // Check that player is now at (6,6)
    auto playerPos = s.playerLoc();
    BOOST_REQUIRE_EQUAL(playerPos.x, 6);
    BOOST_REQUIRE_EQUAL(playerPos.y, 6);
}

BOOST_AUTO_TEST_CASE(Test_BoxBlockedByWall) {
    SB::Sokoban s("level1.lvl");

    // Push box from (6,6) to (7,6), then to (8,6)
    s.movePlayer(SB::Direction::Right);  // (4,6)
    s.movePlayer(SB::Direction::Right);  // (5,6)
    s.movePlayer(SB::Direction::Right);  // push to (7,6)
    s.movePlayer(SB::Direction::Right);  // push to (8,6)

    sf::Vector2u before = s.playerLoc();
    s.movePlayer(SB::Direction::Right);  // attempt push into wall at (9,6)

    BOOST_CHECK(s.playerLoc() == before);  // should not move
}

BOOST_AUTO_TEST_CASE(BoxPushedToCorrectSpot) {
    SB::Sokoban s("level1.lvl");

    // Push box once
    s.movePlayer(SB::Direction::Right);  // (4,6)
    s.movePlayer(SB::Direction::Right);  // (5,6)
    s.movePlayer(SB::Direction::Right);  // (6,6)

    sf::Vector2u playerPos = s.playerLoc();
    BOOST_REQUIRE_EQUAL(playerPos.x, 6);
    BOOST_REQUIRE_EQUAL(playerPos.y, 6);
}

BOOST_AUTO_TEST_CASE(Test_PlayerBlockedOffMap) {
    SB::Sokoban s("level1.lvl");

    // Move left to near the wall
    s.movePlayer(SB::Direction::Left);  // (2,6)
    s.movePlayer(SB::Direction::Left);  // (1,6)

    sf::Vector2u before = s.playerLoc();
    s.movePlayer(SB::Direction::Left);  // (0,6) — wall

    BOOST_CHECK(s.playerLoc() == before);
}

BOOST_AUTO_TEST_CASE(Box_Cannot_Be_Pushed_Off_Screen) {
    SB::Sokoban s("level1.lvl");

    // Push box twice toward wall
    s.movePlayer(SB::Direction::Right);  // (4,6)
    s.movePlayer(SB::Direction::Right);  // (5,6)
    s.movePlayer(SB::Direction::Right);  // push to (7,6)
    s.movePlayer(SB::Direction::Right);  // push to (8,6)

    sf::Vector2u before = s.playerLoc();
    s.movePlayer(SB::Direction::Right);  // attempt push to (9,6) — wall

    BOOST_CHECK(s.playerLoc() == before);  // should not move
}

BOOST_AUTO_TEST_CASE(Box_Cannot_Be_Pushed_Into_Another_Box) {
    SB::Sokoban s("level1.lvl");

    // Walk under the box at (6,6)
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Down);
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Left);
    // Now we are at (6,2), with a box directly to the LEFT at (5,2)
    // Try to push LEFT into that box
    sf::Vector2u before = s.playerLoc();
    // Try to push box from (6,2) into (5,2)
    s.movePlayer(SB::Direction::Left);

    // Should fail, and player should not move
    BOOST_CHECK(s.playerLoc() == before);
}

BOOST_AUTO_TEST_CASE(Lots_Of_Targets) {
    SB::Sokoban s("level6.lvl");
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Left);
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Down);
    s.movePlayer(SB::Direction::Down);
    s.movePlayer(SB::Direction::Down);
    s.movePlayer(SB::Direction::Down);
    s.movePlayer(SB::Direction::Left);
    s.movePlayer(SB::Direction::Down);
    s.movePlayer(SB::Direction::Right);


    BOOST_REQUIRE_EQUAL(s.isWon(), true);
}

BOOST_AUTO_TEST_CASE(Lots_Of_Boxes) {
    SB::Sokoban s("level5.lvl");
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Left);
    s.movePlayer(SB::Direction::Up);
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Right);

    BOOST_REQUIRE_EQUAL(s.isWon(), true);
}

BOOST_AUTO_TEST_CASE(Player_Cannot_Move_Off_Screen) {
    SB::Sokoban s("level4.lvl");

    // Find a wall — let’s just move the player left into the border wall
    // depending on level, this should hit wall
    s.movePlayer(SB::Direction::Up);
    sf::Vector2u before = s.playerLoc();
    s.movePlayer(SB::Direction::Up);
    BOOST_CHECK(s.playerLoc() == before);
    s.movePlayer(SB::Direction::Down);
    s.movePlayer(SB::Direction::Down);
    s.movePlayer(SB::Direction::Down);
    s.movePlayer(SB::Direction::Down);
    s.movePlayer(SB::Direction::Down);
    s.movePlayer(SB::Direction::Right);
    s.movePlayer(SB::Direction::Down);
    s.movePlayer(SB::Direction::Down);
    sf::Vector2u secbefore = s.playerLoc();
    s.movePlayer(SB::Direction::Down);
    BOOST_CHECK(s.playerLoc() == secbefore);
}





