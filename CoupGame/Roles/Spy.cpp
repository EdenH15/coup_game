//
// Created by eden on 5/8/25.
//
#include "Spy.h"

#include <ostream>

#include "../Player.h"
#include "iostream"

namespace CoupG {

    std::string Spy::getName() const {
        return "Spy";
    }

    std::string Spy::useAbility(Player& self, Player& target) {
        const int targetCoins=target.getCoins();
        target.setBlockArrest(true);
        std::cout << self.getName() + " blocked arrest & saw " + target.getName() + " has " + std::to_string(targetCoins) + " coins."<<std::endl;
        return target.getName() + " has " + std::to_string(targetCoins) + " coins.";
    }


}

