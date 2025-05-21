#include "GameGUI.h"

//
// Created by eden on 5/21/25.
//

#include "CoupGame/Player.h"
#include "CoupGame/Roles/Governor.h"
#include "CoupGame/Roles/Spy.h"
#include "CoupGame/Roles/Baron.h"
#include "CoupGame/Roles/General.h"
#include "CoupGame/Roles/Judge.h"
#include "CoupGame/Roles/Merchant.h"
#include "CoupGame/Game.h"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;
using namespace CoupG;

int main() {
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

    vector<string> players = game_1.players();

    // Expected output:
    // Moshe
    // Yossi
    // Meirav
    // Reut
    // Gilad
    for(string name : players){
        cout << name << endl;
    }

    // Expected output: Eden
    cout << game_1.turn() << endl;

    governor->gather();
    spy->gather();
    baron->gather();
    general->gather();
    judge->gather();

    // Expected exception - Not spy's turn
    try{
        spy->gather();
    } catch (const std::exception &e){
        std::cerr << e.what() << '\n';
    }
    cout << game_1.turn() << endl;
    governor->gather();
    cout << game_1.turn() << endl;
    spy->tax();

    cout << governor->getCoins() << endl; // Expected: 2
    cout << spy->getCoins() << endl; // Expected: 3
    governor->useAbility(*spy); // Governor undo tax
    cout << spy->getCoins() << endl; // Expected: 1
    cout << game_1.turn() << endl;

    baron->tax();
    cout << game_1.turn() << endl;
    general->gather();
    cout << game_1.turn() << endl;
    judge->gather();
    cout << game_1.turn() << endl;
    governor->tax();
    cout << game_1.turn() << endl;
    spy->gather();
    cout << game_1.turn() << endl;
    baron->useAbility(); // Baron traded its 3 coins and got 6
    cout << game_1.turn() << endl;
    baron->tax();
    general->gather();
    judge->gather();

    cout << baron->getCoins() << endl; // Expected: 8

    governor->tax();
    spy->gather();
    baron->gather();
    general->gather();
    judge->gather();

    governor->tax();
    spy->gather();
    cout << baron->getCoins() << endl; // Expected: 9
    baron->coup(*governor); // Coup against governor
    general->gather();
    judge->gather();

    players = game_1.players();
    // Since no one blocked the Baron, the expected output is:
    // Yossi
    // Dekel
    // Yuval
    // Ofek
    for (string name : players) {
        cout << name << endl;
    }

}