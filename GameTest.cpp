//
// Created by eden on 5/24/25.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "CoupGame/Roles/Baron.h"
#include "CoupGame/PlayerFactory.h"

using namespace CoupG;

TEST_CASE("Constructor throws on invalid player count") {
    CHECK_THROWS_AS(Game(1), std::invalid_argument);
    CHECK_THROWS_AS(Game(7), std::invalid_argument);
    CHECK_NOTHROW(Game(2));
    CHECK_NOTHROW(Game(6));
}

TEST_CASE("Start and end game set correct flags") {
    Game game(3);
    CHECK(game.isGameActive() == true);
    game.endGame();
    CHECK(game.isGameActive() == false);
}

TEST_CASE("Add players up to limit and overflow") {
    Game game(3);

    Player* p1 = PlayerFactory::createRandomPlayer(game, "Alice");
    Player* p2 = PlayerFactory::createRandomPlayer(game, "Eden");
    Player* p3 = PlayerFactory::createRandomPlayer(game, "Amir");
    Player* p4 = PlayerFactory::createRandomPlayer(game, "Dave");

    CHECK_NOTHROW(game.addPlayer(p1));
    CHECK_NOTHROW(game.addPlayer(p2));
    CHECK_NOTHROW(game.addPlayer(p3));
    // Adding more than numPlayers throws
    CHECK_THROWS_AS(game.addPlayer(p4), std::runtime_error);
    delete p4;

    // Clean up manually because Game destructor expects ownership of added players
    game.reset();
}

TEST_CASE("isPlayerInGame and exceptions") {
    Game game(2);
    Player* p1 = PlayerFactory::createRandomPlayer(game, "Eden");
    Player* p2 = PlayerFactory::createRandomPlayer(game, "Lior");

    game.addPlayer(p1);
    game.addPlayer(p2);

    CHECK(game.isPlayerInGame(p1));
    CHECK(game.isPlayerInGame(p2));

    Player* p3 = PlayerFactory::createRandomPlayer(game, "NotInGame");
    CHECK_THROWS_AS(game.isPlayerInGame(p3), std::invalid_argument);

    game.reset();
    delete p3;

}

TEST_CASE("nextTurn and current player logic with active and inactive players") {
    Game game(3);
    Player* p1 = PlayerFactory::createRandomPlayer(game, "Player1");
    Player* p2 = PlayerFactory::createRandomPlayer(game, "Player2");
    Player* p3 = PlayerFactory::createRandomPlayer(game, "Player3");

    game.addPlayer(p1);
    game.addPlayer(p2);
    game.addPlayer(p3);

    p1->setActive(true);
    p2->setActive(false);
    p3->setActive(true);

    // Start from player 0 (p1)
    CHECK(game.getCurrentPlayer() == 0);
    CHECK(game.isCurrentPlayer(p1));

    game.nextTurn(); // Should skip p2 (inactive) and go to p3
    CHECK(game.getCurrentPlayer() == 2);
    CHECK(game.isCurrentPlayer(p3));

    // Deactivate p3, only p1 is active
    p3->setActive(false);
    game.nextTurn(); // Should return to p1 since only active player
    CHECK(game.getCurrentPlayer() == 0);

    // Remove all active players => nextTurn throws
    p1->setActive(false);
    CHECK_THROWS_AS(game.nextTurn(), std::runtime_error);

    game.reset();
}

TEST_CASE("getWinner throws if game active and returns winner if ended") {
    Game game(2);
    Player* p1 = PlayerFactory::createRandomPlayer(game, "Winner");
    Player* p2 = PlayerFactory::createRandomPlayer(game, "Loser");

    game.addPlayer(p1);
    game.addPlayer(p2);

    CHECK_THROWS_AS(game.getWinner(), std::runtime_error);

    // Simulate game end and winner set
    game.endGame();
    game.setUnderArrest("");
    game.removePlayer(*p2); // p1 active, p2 inactive
    game.checkForWinner();

    // Now winner should be "Winner"
    std::string winnerName = game.getWinner();
    CHECK(winnerName == p1->getName());

    game.reset();
}

TEST_CASE("getWinner throws if game active and returns winner if ended") {
    Game game(2);
    Player* p1 = PlayerFactory::createRandomPlayer(game, "Winner");
    Player* p2 = PlayerFactory::createRandomPlayer(game, "Loser");

    game.addPlayer(p1);
    game.addPlayer(p2);

    CHECK_THROWS_AS(game.getWinner(), std::runtime_error);

    // Simulate game end and winner set
    game.removePlayer(*p2);
    game.endGame();
     // p1 active, p2 inactive
    game.checkForWinner();

    // Now winner should be "Winner"
    std::string winnerName = game.getWinner();
    CHECK(winnerName == p1->getName());

    game.reset();
}

TEST_CASE("validateTurnStart behavior and exceptions") {
    Game game(2);
    Player* p1 = PlayerFactory::createRandomPlayer(game, "ActivePlayer");
    Player* p2 = PlayerFactory::createRandomPlayer(game, "InactivePlayer");

    game.addPlayer(p1);
    game.addPlayer(p2);


    p1->setActive(true);
    p2->setActive(false);


    CHECK_NOTHROW(game.validateTurnStart(*p1));
    CHECK_THROWS_AS(game.validateTurnStart(*p2), std::runtime_error);


    game.nextTurn();
    CHECK_THROWS_AS(game.validateTurnStart(*p2), std::runtime_error);
    CHECK_NOTHROW(game.validateTurnStart(*p1));


    p1->setCoins(10);
    CHECK_THROWS_AS(game.validateTurnStart(*p1), std::runtime_error);

    game.reset();
}


TEST_CASE("players() returns only active players' names") {
    Game game(4);
    Player* p1 = PlayerFactory::createRandomPlayer(game, "P1");
    Player* p2 = PlayerFactory::createRandomPlayer(game, "P2");
    Player* p3 = PlayerFactory::createRandomPlayer(game, "P3");
    Player* p4 = PlayerFactory::createRandomPlayer(game, "P4");

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


