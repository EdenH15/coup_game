//
// Created by eden on 5/8/25.
//
#include "Baron.h"

#include "../Player.h"
#include <iostream>

namespace CoupG {

    std::string Baron::getName() const {
        return "Baron";
    }

    std::string Baron::useAbility(Player& self,Player& target) {
        (void)target;
        if (self.getCoins() < 3) {
            return self.getName() + " does not have enough coins";
        }

        self.setCoins(-3);
        self.setCoins(6);

        return self.getName() + " invested 3 coins and received 6 coins!";
    }

    void Baron::onSanction(Player& target) override {
        target.setCoins(1);
        std::cout << target.getName() << " was sanctioned but received 1 coin compensation as Baron.\n";
    }

}
