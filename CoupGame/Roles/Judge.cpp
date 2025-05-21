//
// Created by eden on 5/8/25.
//
#include "Judge.h"
#include "../Player.h"
#include <iostream>

namespace CoupG {

    Judge::Judge(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Judge";
    }

    void Judge::useAbility(Player& p) const {
        if (!this->active) {
            throw std::runtime_error("Inactive player can't invest.");
        }
        if (p.getLastAction()==ActionType::Bribe) {
            throw std::invalid_argument("Judge can only undo bribe actions.");
        }
        p.setAnotherTurn(false);
        p.setLastAction(ActionType::None);
    }

    void Judge::receiveSanctionBy(Player& p) {
        Player::receiveSanctionBy(p);
        if (p.getCoins() > 0) {
            p.setCoins( p.getCoins()-1);
        } else {
            std::cout << p.getName() << " had no coin to pay as penalty for sanctioning Judge.\n";
        }
    }

}