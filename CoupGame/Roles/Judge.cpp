//
// Created by eden on 5/8/25.
//
#include "Judge.h"
#include "../Player.h"
#include <iostream>

namespace CoupG {

    std::string Judge::getName()const {
        return "Judge";
    }

    std::string useAbility(Player& self, Player& target) {
        if (target.getLastAction()==ActionType::Bribe) {
            return self.getName() + " canceled " + target.getName() + "bribe.";
        }
        return "Cannot canceled " + target.getName() + " last action was not a bribe.";
    }

    void onSanction(Player& target) {
        target.setCoins(-1);
        std::cout << target.getName() + " sanctioned a Judge and paid 1 extra coin." << std::endl;

    }

}