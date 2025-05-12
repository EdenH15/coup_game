//
// Created by eden on 5/8/25.
//

#include "Merchant.h"
#include <iostream>
#include "../Player.h"

namespace CoupG {
    std::string Merchant::getName() const {
        return "Merchant";
    }

    std::string Merchant::useAbility(Player &self, Player &target) {
        (void)target;
        if (self.getCoins() >= 3) {
            self.setCoins(1);
            std::cout << self.getName() + " started the turn with at least 3 coins and gained 1 bonus coin." <<
                    std::endl;
            return self.getName() + "gained 1 bonus coin.";
        }
        std::cout << self.getName() + " has less than 3 coins. No bonus granted." << std::endl;
        return self.getName() + "No bonus granted.";
    }

    void onArrest(Player &target) {
        if (target.getLastAction() == ActionType::Arrest) {
            target.setCoins(-2);
            std::cout << target.getName() + " was arrested and paid " << std::endl;
        }
    }
}
