#include "Gui/GameGUI.h"
#include "CoupGame/Player.h"
#include "CoupGame/Roles/Governor.h"
#include "CoupGame/Roles/Spy.h"
#include "CoupGame/Roles/Baron.h"
#include "CoupGame/Roles/General.h"
#include "CoupGame/Roles/Judge.h"
#include "CoupGame/Game.h"

#include <iostream>
#include <vector>
#include <exception>
using namespace std;
using namespace CoupG;

void printTitle(const string& title) {
    cout << "\n========== " << title << " ==========\n" << endl;
}

int main() {
    printTitle("Starting Coup Game");
    Game game_1(5);

    Player* governor = new Governor(game_1, "Eden");
    Player* spy = new Spy(game_1, "Yossi");
    Player* baron = new Baron(game_1, "Dekel");
    Player* general = new General(game_1, "Yuval");
    Player* judge = new Judge(game_1, "Offer");

    game_1.addPlayer(governor);
    game_1.addPlayer(spy);
    game_1.addPlayer(baron);
    game_1.addPlayer(general);
    game_1.addPlayer(judge);

    printTitle("Players in the game");
    for (const string& name : game_1.players()) {
        cout << "- " << name << endl;
    }

    cout << "\nCurrent turn: " << game_1.turn() << endl;

    cout << governor->getName() << " gathers coins." << endl;
    governor->gather();

    cout << spy->getName() << " gathers coins." << endl;
    spy->gather();

    cout << baron->getName() << " gathers coins." << endl;
    baron->gather();

    cout << general->getName() << " gathers coins." << endl;
    general->gather();

    cout << judge->getName() << " gathers coins." << endl;
    judge->gather();

    cout << spy->getName() << " tries to gather again (should fail):" << endl;
    try {
        spy->gather();
    } catch (const exception& e) {
        cout << "Error: " << e.what() << " (Player: " << spy->getName() << ", Action: gather)" << endl;
    }

    cout << "\nCurrent turn: " << game_1.turn() << endl;

    cout << governor->getName() << " gathers coins." << endl;
    governor->gather();

    cout << "\nCurrent turn: " << game_1.turn() << endl;

    cout << spy->getName() << " uses tax action." << endl;
    spy->tax();

    cout << governor->getName() << " has " << governor->getCoins() << " coins." << endl;
    cout << spy->getName() << " has " << spy->getCoins() << " coins." << endl;

    cout << governor->getName() << " uses ability to undo " << spy->getName() << "'s tax." << endl;
    governor->useAbility(*spy);

    cout << spy->getName() << " now has " << spy->getCoins() << " coins." << endl;
    cout << "\nCurrent turn: " << game_1.turn() << endl;

    cout << baron->getName() << " uses tax action." << endl;
    baron->tax();

    cout << "\nCurrent turn: " << game_1.turn() << endl;

    cout << general->getName() << " gathers coins." << endl;
    general->gather();

    cout << "\nCurrent turn: " << game_1.turn() << endl;

    cout << judge->getName() << " gathers coins." << endl;
    judge->gather();

    cout << "\nCurrent turn: " << game_1.turn() << endl;

    cout << governor->getName() << " uses tax." << endl;
    governor->tax();

    cout << "\nCurrent turn: " << game_1.turn() << endl;

    cout << spy->getName() << " gathers coins." << endl;
    spy->gather();

    cout << "\nCurrent turn: " << game_1.turn() << endl;

    //Baron invest 3 coins and get 6 coins
    cout << baron->getName() << " uses special ability(Invest)." << endl;
    baron->useAbility();

    cout << "\nCurrent turn: " << game_1.turn() << endl;

    cout << baron->getName() << " uses tax." << endl;
    baron->tax();
    cout << baron->getName() << " now has " << baron->getCoins() << " coins." << endl;

    cout << general->getName() << " gathers coins." << endl;
    general->gather();

    cout << judge->getName() << " gathers coins." << endl;
    judge->gather();


    cout << governor->getName() << " uses tax." << endl;
    governor->tax();

    cout << spy->getName() << " gathers coins." << endl;
    spy->gather();

    cout << baron->getName() << " gathers coins." << endl;
    baron->gather();

    cout << general->getName() << " gathers coins." << endl;
    general->gather();

    cout << judge->getName() << " gathers coins." << endl;
    judge->gather();

    cout << governor->getName() << " uses tax." << endl;
    governor->tax();

    cout << spy->getName() << " uses tax." << endl;
    spy->tax();

    cout << baron->getName() << " has " << baron->getCoins() << " coins." << endl;

    cout << baron->getName() << " performs coup on " << governor->getName() << "." << endl;
    baron->coup(*governor);

    cout << general->getName() << " uses tax." << endl;
    general->tax();

    cout << judge->getName() << " uses tax." << endl;
    judge->tax();

    printTitle("Remaining players");
    for (Player* p : game_1.getAllPlayers()) {
        if (p->getActive()) {
            cout << "- " << p->getName() << " (" << p->getCoins() << " coins)" << endl;
        }
    }

    spy->tax();
    baron->tax();
    general->tax();
    judge->tax();

    spy->coup(*baron);
    cout << "\n"<<spy->getName() << " performs coup on " << baron->getName() << "." << endl;
    general->coup(*judge);
    cout << general->getName() << " performs coup on " << judge->getName() << "." << endl;

    game_1.checkForWinner();
    try {
        const std::string winner = game_1.getWinner();
        cout << "The winner is:" << winner << endl;
    }
    catch (...) {
        cout<< "There is no winner,Game is still active"<<endl;
    }
    spy->setCoins(7);
    spy->coup(*general);
    cout << spy->getName() << " performs coup on " << general->getName() << "." << endl;

    game_1.checkForWinner();
    try {
        const std::string winner = game_1.getWinner();
        cout << "The winner is:" << winner << endl;
    }
    catch (...) {
        cout<< "There is no winner,Game is still active"<<endl;
    }

    printTitle("Game Ended - Cleaning Up");
    game_1.reset();

    return 0;
}
