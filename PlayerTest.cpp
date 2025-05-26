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
    governor->tax();
    CHECK(governor->getCoins()==3);
    merchant->tax();
    CHECK(merchant->getCoins()==2);
    governor->useAbility(*merchant);
    CHECK(merchant->getCoins()==0);
    spy->tax();
    CHECK(spy->getCoins()==2);
    game.reset();
}

TEST_CASE("Gather+Sanction+Tax action") {
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
    CHECK_THROWS_AS(governor->sanction(*judge), std::runtime_error);
    governor->setCoins(4);
    governor->sanction(*judge);
    CHECK(governor->getCoins()==0);
    CHECK_THROWS_AS(judge->gather(), std::runtime_error);
    CHECK_THROWS_AS(judge->tax(), std::runtime_error);
    judge->setCoins(3);
    judge->sanction(*baron);
    CHECK(baron->getCoins()==2);
}
