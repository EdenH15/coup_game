//
// Created by eden on 5/8/25.
//
#include "Spy.h"

#include <ostream>

#include "../Player.h"
#include "iostream"

namespace CoupG {
    Spy::Spy(Game &game, const std::string &name)
        : Player(game, name) {
        role = "Spy";
    }

    int Spy::useSpyAbility(Player &p) {
        if (!this->active) {
            throw std::runtime_error("Inactive players can't use abilities.");
        }

        if (!p.getActive()) {
            throw std::runtime_error("Target player is not active.");
        }

        p.setBlockArrest(true);
        return p.getCoins();
    }
}
