//
// Created by eden on 5/8/25.
//
#include "Baron.h"
#include "../Player.h"
#include <iostream>
#include <string>

namespace CoupG {

    Baron::Baron(Game& game, const std::string& name)
        : Player(game, name) {
        role = "Baron";
    }


    void Baron::useAbility()  {
        if (!this->active) {
            throw std::runtime_error("Inactive player can't invest.");
        }

        if (this->coins < 3) {
            throw std::runtime_error("Not enough coins to invest (need 3).");
        }

        this->coins -= 3;
        this->coins += 6;
    }

    void Baron::receiveSanctionBy(Player& p) {
        this->underSanction = true;
        this->coins += 1;
    }

}
