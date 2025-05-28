//
// Created by eden on 5/8/25.
// Email:Edenhassin@gmail.com

#include "General.h"
#include "../Player.h"
#include <iostream>

namespace CoupG {
    General::General(Game &game, const std::string &name)
        : Player(game, name) {
        role = "General";
    }

    void General::useAbility(Player &p) {
        if (!this->getActive()) {
            throw std::runtime_error("Inactive general cannot block.");
        }

        if (this->getCoins() < 5) {
            throw std::runtime_error("2");
        }

        coins -= 5;
        p.setActive(true);
    }

    void General::receiveArrestBy(Player &p) {
        p.setCoins(p.getCoins() + 1);
    }
}
