//
// Created by eden on 5/24/25.
// Email:Edenhassin@gmail.com

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../CoupGame/Roles/Baron.h"
#include "../CoupGame/PlayerFactory.h"

using namespace CoupG;

/**
 * Test constructor validation for player count.
 */
TEST_CASE("Constructor") {
    CHECK_THROWS_AS(Game(1), std::invalid_argument); // Less than minimum (2)
    CHECK_THROWS_AS(Game(7), std::invalid_argument); // More than maximum (6)
    CHECK_NOTHROW(Game(2)); // Minimum valid
    CHECK_NOTHROW(Game(6)); // Maximum valid
}

/**
 * Test starting and ending the game.
 */
TEST_CASE("Start and end game") {
    Game game(3);
    CHECK(game.isGameActive() == true); // Game should be active after construction
    game.endGame();
    CHECK(game.isGameActive() == false); // Game should be inactive after ending
}

/**
 * Test adding players to the game.
 */
TEST_CASE("Add players") {
    Game game(3);

    Player *p1 = PlayerFactory::createRandomPlayer(game, "Alice");
    Player *p2 = PlayerFactory::createRandomPlayer(game, "Eden");
    Player *p3 = PlayerFactory::createRandomPlayer(game, "Amir");
    Player *p4 = PlayerFactory::createRandomPlayer(game, "David");

    CHECK_NOTHROW(game.addPlayer(p1));
    CHECK_NOTHROW(game.addPlayer(p2));
    CHECK_NOTHROW(game.addPlayer(p3));
    CHECK_THROWS_AS(game.addPlayer(p4), std::runtime_error); // Exceed player limit

    delete p4; // Not added to game, needs manual cleanup
    game.reset(); // Game owns p1-p3 and deletes them
}

/**
 * Test if players are recognized as in-game.
 */
TEST_CASE("Is player in game") {
    Game game(2);
    Player *p1 = PlayerFactory::createRandomPlayer(game, "Eden");
    Player *p2 = PlayerFactory::createRandomPlayer(game, "Lior");

    game.addPlayer(p1);
    game.addPlayer(p2);

    CHECK(game.isPlayerInGame(p1));
    CHECK(game.isPlayerInGame(p2));

    Player *p3 = PlayerFactory::createRandomPlayer(game, "NotInGame");
    CHECK_THROWS_AS(game.isPlayerInGame(p3), std::invalid_argument); // Not added to game

    game.reset();
    delete p3; // Manually delete since not added
}

/**
 * Test turn advancement and active player handling.
 */
TEST_CASE("Next turn and active player logic") {
    Game game(3);
    Player *p1 = PlayerFactory::createRandomPlayer(game, "Player1");
    Player *p2 = PlayerFactory::createRandomPlayer(game, "Player2");
    Player *p3 = PlayerFactory::createRandomPlayer(game, "Player3");

    game.addPlayer(p1);
    game.addPlayer(p2);
    game.addPlayer(p3);

    p1->setActive(true);
    p2->setActive(false);
    p3->setActive(true);

    CHECK(game.getCurrentPlayer() == 0);
    CHECK(game.isCurrentPlayer(p1));

    game.nextTurn(); // Should skip p2
    CHECK(game.getCurrentPlayer() == 2);
    CHECK(game.isCurrentPlayer(p3));

    p3->setActive(false);
    game.nextTurn(); // Only p1 is active
    CHECK(game.getCurrentPlayer() == 0);

    p1->setActive(false);
    CHECK_THROWS_AS(game.nextTurn(), std::runtime_error); // No active players

    game.reset();
}

/**
 * Test getting the winner of the game.
 */
TEST_CASE("Get winner") {
    Game game(2);
    Player *p1 = PlayerFactory::createRandomPlayer(game, "Winner");
    Player *p2 = PlayerFactory::createRandomPlayer(game, "Loser");

    game.addPlayer(p1);
    game.addPlayer(p2);

    CHECK_THROWS_AS(game.getWinner(), std::runtime_error); // Game not over yet

    game.endGame();
    game.setUnderArrest("");
    game.removePlayer(*p2); // p2 removed
    game.checkForWinner();

    std::string winnerName = game.getWinner();
    CHECK(winnerName == p1->getName());

    game.reset();
}

/**
 * Test validation before a player starts their turn.
 */
TEST_CASE("validateTurnStart") {
    Game game(2);
    Player *p1 = PlayerFactory::createRandomPlayer(game, "ActivePlayer");
    Player *p2 = PlayerFactory::createRandomPlayer(game, "InactivePlayer");

    game.addPlayer(p1);
    game.addPlayer(p2);

    p1->setActive(true);
    p2->setActive(false);

    CHECK_NOTHROW(game.validateTurnStart(*p1));
    CHECK_THROWS_AS(game.validateTurnStart(*p2), std::runtime_error); // Not active

    game.nextTurn();
    CHECK_THROWS_AS(game.validateTurnStart(*p2), std::runtime_error); // Still inactive
    CHECK_NOTHROW(game.validateTurnStart(*p1));

    p1->setCoins(10); // Too many coins, must coup
    CHECK_THROWS_AS(game.validateTurnStart(*p1), std::runtime_error);

    game.reset();
}

/**
 * Test players() returns only active player names.
 */
TEST_CASE("players() returns only active players") {
    Game game(4);
    Player *p1 = PlayerFactory::createRandomPlayer(game, "P1");
    Player *p2 = PlayerFactory::createRandomPlayer(game, "P2");
    Player *p3 = PlayerFactory::createRandomPlayer(game, "P3");
    Player *p4 = PlayerFactory::createRandomPlayer(game, "P4");

    game.addPlayer(p1);
    game.addPlayer(p2);
    game.addPlayer(p3);
    game.addPlayer(p4);

    p1->setActive(true);
    p2->setActive(false);
    p3->setActive(true);
    p4->setActive(false);

    std::vector<std::string> activePlayers = game.players();

    CHECK(activePlayers.size() == 2);
    CHECK(activePlayers[0] == p1->getName());
    CHECK(activePlayers[1] == p3->getName());

    game.reset();
}

/**
 * Test removing a player from the game.
 */
TEST_CASE("Remove player") {
    Game game(3);
    Player *p1 = PlayerFactory::createRandomPlayer(game, "P1");
    Player *p2 = PlayerFactory::createRandomPlayer(game, "P2");
    Player *p3 = PlayerFactory::createRandomPlayer(game, "P3");

    game.addPlayer(p1);
    game.addPlayer(p2);
    game.addPlayer(p3);

    CHECK(game.isPlayerInGame(p2));
    game.removePlayer(*p2);
    CHECK(game.isPlayerInGame(p2)==false);
    CHECK(p2->getActive() == false); // Player should be marked inactive

    game.reset();
}


TEST_CASE("PlayerFactory::createRandomPlayer returns a valid player with a valid role") {
    Game game(3);
    std::vector<std::string> validRoles = {
        "Baron", "Spy", "General", "Governor", "Merchant", "Judge"
    };


    Player *p = PlayerFactory::createRandomPlayer(game, "Alice");
    REQUIRE(p != nullptr);
    std::string role = p->getRole();
    std::cout << "Role returned: " << role << std::endl; // הדפסת התפקיד שנבחר

    CHECK(std::find(validRoles.begin(), validRoles.end(), role) != validRoles.end());
    delete p;
}
