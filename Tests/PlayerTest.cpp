//Email:Edenhassin@gmail.com

#include "doctest.h"
#include "../CoupGame/Game.h"
#include "../CoupGame/Player.h"
#include "../CoupGame/Roles/Baron.h"
#include "../CoupGame/Roles/Governor.h"
#include "../CoupGame/Roles/General.h"
#include "../CoupGame/Roles/Judge.h"
#include <stdexcept>
#include "../CoupGame/Roles/Merchant.h"
#include "../CoupGame/Roles/Spy.h"
using namespace CoupG;


/**
 * Test tax action behavior for multiple roles, including ability interactions.
 */
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
    CHECK(baron->getCoins() == 2);

    // Governor gains 3 coins from tax
    governor->tax();
    CHECK(governor->getCoins() == 3);


    merchant->tax();
    CHECK(merchant->getCoins() == 2);

    // Governor can reverse the Merchant's tax action using ability
    governor->useAbility(*merchant);
    CHECK(merchant->getCoins() == 0);

    spy->tax();
    CHECK(spy->getCoins() == 2);

    game.reset();
}


/**
 * Test gather and sanction actions, including coin requirements and effects.
 */
TEST_CASE("Gather + Sanction action") {
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
    CHECK(baron->getCoins() == 1);

    // Should fail because the Governor doesn't have enough coins to use sanction
    CHECK_THROWS_AS(governor->sanction(*judge), std::runtime_error);

    governor->setCoins(4);
    governor->sanction(*judge);
    CHECK(governor->getCoins() == 0);

    // Judge is under sanction and cannot perform actions
    CHECK_THROWS_AS(judge->gather(), std::runtime_error);
    CHECK_THROWS_AS(judge->tax(), std::runtime_error);

    judge->setCoins(3);
    judge->sanction(*baron);

    // Baron receives 1 coin when targeted by sanction
    CHECK(baron->getCoins() == 2);

    game.reset();
}

/**
 * Test arrest logic, including required conditions and special abilities.
 */
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

    // Should fail because the General has less than 1 coin and cannot be arrested
    CHECK_THROWS_AS(baron->arrest(*general), std::runtime_error);

    general->setCoins(2);
    baron->arrest(*general);

    // General retains his coins after being arrested
    CHECK(general->getCoins() == 2);

    spy->setCoins(1);
    general->arrest(*spy);

    // Should fail because the Spy was the most recent target of arrest
    CHECK_THROWS_AS(merchant->arrest(*spy), std::runtime_error);

    merchant->tax();

    // Spy blocks arrest using their special ability
    spy->useSpyAbility(*baron);

    // Merchant pays 2 coins as a penalty instead of losing 1 coin to the arresting player
    merchant->setCoins(2);
    spy->arrest(*merchant);
    CHECK(merchant->getCoins() == 0);
    CHECK(spy->getCoins() == 0);

    // Baron is blocked from performing arrests
    CHECK_THROWS_AS(baron->arrest(*general), std::runtime_error);

    game.reset();
}

/**
 * Test bribe action, including failed attempts and interactions with abilities.
 */
TEST_CASE("Bribe action") {
    Game game(3);
    Player *baron = new Baron(game, "Dekel");
    Player *governor = new Governor(game, "Eden");
    Player *judge = new Judge(game, "Offer");

    game.addPlayer(baron);
    game.addPlayer(governor);
    game.addPlayer(judge);

    // Should fail because the Baron doesn't have enough coins to bribe
    CHECK_THROWS_AS(baron->bribe(), std::runtime_error);

    baron->tax();

    // Should fail because the Baron lost their turn due to a previous action
    CHECK_THROWS_AS(baron->gather(), std::runtime_error);

    governor->setCoins(4);
    governor->bribe();

    // Judge cancels the Governor's bribe using ability
    judge->useAbility(*governor);
    CHECK(governor->getCoins() == 0);

    governor->tax();

    // Should fail because the bribe was cancelled by the Judge
    CHECK_THROWS_AS(governor->gather(), std::runtime_error);

    judge->setCoins(4);
    judge->bribe();
    judge->tax();

    // Judge gains an additional turn
    judge->gather();

    game.reset();
}


/**
 * Test coup action behavior, including coin checks and role-based ability responses.
 */
TEST_CASE("Coup action") {
    Game game(4);
    Player *baron = new Baron(game, "Dekel");
    Player *general = new General(game, "Eden");
    Player *merchant = new Merchant(game, "Offer");

    game.addPlayer(baron);
    game.addPlayer(general);
    game.addPlayer(merchant);

    // Should fail because the Baron doesn't have enough coins to perform a coup
    CHECK_THROWS_AS(baron->coup(*merchant), std::runtime_error);

    baron->setCoins(7);
    baron->coup(*merchant);
    CHECK(merchant->getActive() == false);

    // Should fail because the General doesn't have enough coins to prevent the coup
    CHECK_THROWS_AS(general->useAbility(*merchant), std::runtime_error);

    general->setCoins(5);

    // General successfully prevents the coup using their ability
    general->useAbility(*merchant);
    CHECK(merchant->getActive() == true);
    CHECK(baron->getCoins() == 0);

    game.reset();
}


/**
 * Test basic getters and setters for player attributes.
 */
TEST_CASE("Getters and Setters") {
    Game game(2);
    Player *baron = new Baron(game, "Dekel");
    game.addPlayer(baron);

    baron->setCoins(5);
    CHECK(baron->getCoins() == 5);

    baron->setActive(false);
    CHECK(baron->getActive() == false);

    baron->setUnderSanction(true);
    CHECK(baron->isUnderSanction() == true);

    baron->setBlockArrest(true);
    CHECK(baron->isBlockArrest() == true);

    baron->setAnotherTurn(1);
    CHECK(baron->getAnotherTurn() == 1);

    baron->setLastAction(ActionType::Tax);
    CHECK(baron->getLastAction() == ActionType::Tax);

    CHECK(baron->getName() == "Dekel");
    game.reset();
}


/**
 * Test direct calls to receiveSanctionBy and receiveArrestBy methods.
 */
TEST_CASE("Direct sanction/arrest effects") {
    Game game(2);
    Player *baron = new Baron(game, "Dekel");
    Player *governor = new Governor(game, "Eden");
    game.addPlayer(baron);
    game.addPlayer(governor);

    baron->setCoins(3);
    CHECK_FALSE(governor->isUnderSanction());
    governor->receiveSanctionBy(*baron);
    CHECK(governor->isUnderSanction() == true);

    governor->setCoins(3);
    baron->setCoins(0);
    governor->receiveArrestBy(*baron);
    CHECK(governor->getCoins() == 2);
    CHECK(baron->getCoins() == 1);
    game.reset();
}

/**
 * Test the behavior of the lastAction attribute, including updates and manual setting.
 */
TEST_CASE("Player lastAction indicator behavior") {
    Game game(2);
    Player *baron = new Baron(game, "Dekel");
    Player *spy = new Spy(game, "Yossi");

    game.addPlayer(baron);
    game.addPlayer(spy);

    SUBCASE("Initial lastAction is None") {
        // At the beginning, before any action is performed, lastAction should be None
        CHECK(baron->getLastAction() == ActionType::None);
    }

    SUBCASE("Valid actions update lastAction") {
        // Performing a valid tax action should update lastAction to Tax
        baron->tax();
        CHECK(baron->getLastAction() == ActionType::Tax);

        // Performing gather should update lastAction to Gather
        spy->gather();
        CHECK(spy->getLastAction() == ActionType::Gather);
    }

    SUBCASE("Failed actions do NOT update lastAction") {
        // Try to perform a bribe with insufficient coins — should throw and not update lastAction
        CHECK_THROWS(baron->bribe());
        CHECK(baron->getLastAction() == ActionType::None); // Should still be None

        // Perform a valid action to initialize lastAction
        baron->tax();
        CHECK(baron->getLastAction() == ActionType::Tax);

        // Now perform an invalid coup with 0 coins, should throw and keep lastAction unchanged
        spy->setCoins(0);
        CHECK_THROWS(spy->coup(*baron));
        CHECK(spy->getLastAction() == ActionType::None); // Should remain None
    }

    SUBCASE("Manual setLastAction works") {
        // Test the manual setter and getter of lastAction
        baron->setLastAction(ActionType::Bribe);
        CHECK(baron->getLastAction() == ActionType::Bribe);

        // Reset lastAction to None manually
        baron->setLastAction(ActionType::None);
        CHECK(baron->getLastAction() == ActionType::None);
    }

    game.reset();
}
