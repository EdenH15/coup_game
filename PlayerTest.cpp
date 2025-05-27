#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "CoupGame/Game.h"
#include "CoupGame/Player.h"
#include "CoupGame/Roles/Baron.h"
#include "CoupGame/Roles/Governor.h"
#include "CoupGame/Roles/General.h"
#include "CoupGame/Roles/Judge.h"
#include <stdexcept>

#include "CoupGame/Roles/Merchant.h"
#include "CoupGame/Roles/Spy.h"
using namespace CoupG;

TEST_CASE("Tax action") {
    Game game(4);
    Player *baron = new Baron(game, "Dekel");
    Player *governor = new Governor(game, "Eden");
    Player *merchant = new Merchant(game, "Offer");
    Player *spy = new Spy(game, "Yossi");

    game.addPlayer(baron);
    game.addPlayer(governor);
    game.addPlayer(merchant);
    game.addPlayer(spy);

    baron->tax();
    CHECK(baron->getCoins()==2);

    //Governor get 3 coin
    governor->tax();
    CHECK(governor->getCoins()==3);
    merchant->tax();
    CHECK(merchant->getCoins()==2);

    //Governor can undo the tax
    governor->useAbility(*merchant);
    CHECK(merchant->getCoins()==0);
    spy->tax();
    CHECK(spy->getCoins()==2);
    game.reset();
}

TEST_CASE("Gather+Sanction action") {
    Game game(4);
    Player *baron = new Baron(game, "Dekel");
    Player *governor = new Governor(game, "Eden");
    Player *judge = new Judge(game, "Offer");
    Player *spy = new Spy(game, "Yossi");

    game.addPlayer(baron);
    game.addPlayer(governor);
    game.addPlayer(judge);
    game.addPlayer(spy);

    baron->gather();
    CHECK(baron->getCoins()==1);

    //Failed because the player does not have enough coins to attack
    CHECK_THROWS_AS(governor->sanction(*judge), std::runtime_error);
    governor->setCoins(4);
    governor->sanction(*judge);
    CHECK(governor->getCoins()==0);

    //Judge under sanction
    CHECK_THROWS_AS(judge->gather(), std::runtime_error);
    CHECK_THROWS_AS(judge->tax(), std::runtime_error);
    judge->setCoins(3);
    judge->sanction(*baron);
    //Baron get 1 coin if he attacked by sanction
    CHECK(baron->getCoins()==2);
    game.reset();
}

TEST_CASE("Arrest action") {
    Game game(4);
    Player *baron = new Baron(game, "Dekel");
    Player *general = new General(game, "Eden");
    Player *merchant = new Merchant(game, "Offer");
    Player *spy = new Spy(game, "Yossi");

    game.addPlayer(baron);
    game.addPlayer(general);
    game.addPlayer(merchant);
    game.addPlayer(spy);

    //Failed because the general doesn't have 1 or more coins
    CHECK_THROWS_AS(baron->arrest(*general), std::runtime_error);

    general->setCoins(2);
    baron->arrest(*general);

    //General gets his coin back
    CHECK(general->getCoins()==2);
    general->tax();

    spy->setCoins(1);
    merchant->arrest(*spy);

    //Spy can block arrest
    spy->useSpyAbility(*baron);
    //Merchant pays 2 coins instead of losing 1 to another player
    merchant->setCoins(2);
    spy->arrest(*merchant);
    CHECK(merchant->getCoins()==0);
    CHECK(spy->getCoins()==0);

    //Baron under block arrest
    CHECK_THROWS_AS(baron->arrest(*general), std::runtime_error);
    game.reset();
}

TEST_CASE("Bribe action") {
    Game game(3);
    Player *baron = new Baron(game, "Dekel");
    Player *governor = new Governor(game, "Eden");
    Player *judge = new Judge(game, "Offer");

    game.addPlayer(baron);
    game.addPlayer(governor);
    game.addPlayer(judge);

    //Failed because the player does not have enough coins to bribe
    CHECK_THROWS_AS(baron->bribe(), std::runtime_error);
    baron->tax();

    //Failed because the Baron doesn't have one more turn
    CHECK_THROWS_AS(baron->gather(), std::runtime_error);

    governor->setCoins(4);
    governor->bribe();
    //Judge cancel governor bribe
    judge->useAbility(*governor);
    CHECK(governor->getCoins()==0);
    governor->tax();
    //Failed because judge cancel the bribe
    CHECK_THROWS_AS(governor->gather(), std::runtime_error);
    judge->setCoins(4);
    judge->bribe();
    judge->tax();
    //Judge have one more turn
    judge->gather();
    game.reset();
}

TEST_CASE("Coup action") {
    Game game(4);
    Player *baron = new Baron(game, "Dekel");
    Player *general = new General(game, "Eden");
    Player *merchant = new Merchant(game, "Offer");

    game.addPlayer(baron);
    game.addPlayer(general);
    game.addPlayer(merchant);


    //Failed because the player does not have enough coins to coup
    CHECK_THROWS_AS(baron->coup(*merchant), std::runtime_error);
    baron->setCoins(7);


    baron->coup(*merchant);
    CHECK(merchant->getActive()==false);

    ////Failed because the general does not have enough coins to prevent coup
    CHECK_THROWS_AS(general->useAbility(*merchant), std::runtime_error);
    general->setCoins(5);

    //General prevent a coup
    general->useAbility(*merchant);
    CHECK(merchant->getActive()==true);
    CHECK(baron->getCoins()==0);

    game.reset();

}

