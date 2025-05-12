//
// Created by eden on 5/8/25.
//

#include "General.h"
#include "../Player.h"
#include <iostream>

namespace CoupG {

    std::string General::useAbility(Player& self, Player& target) {
        if (self.getCoins() < 5) {
            return self.getName() + " does not have enough coins to prevent a coup.";
        }
        self.setCoins(-5);
        target.setActive(true);
        std::cout << self.getName() + " blocked a coup and lost 5 coins." << std::endl;
        return self.getName() + " blocked a coup and lost 5 coins.";
    }

    void General::onArrest(Player& target) {
        target.setCoins(1);
        std::cout << target.getName() + " was arrested but got 1 coin back." << std::endl;
    }

}