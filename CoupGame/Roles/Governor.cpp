//
// Created by eden on 5/8/25.
//

#include "Governor.h"

#include "../Player.h"

namespace CoupG {

    std::string Governor::getName() const {
        return "Governor";
    }

    int Governor::onTax() {
        return 3;
    }

    std::string Governor::useAbility(Player& self, Player& target) {
        if (target.getLastAction()==ActionType::Tax) {
            target.setCoins(-target.getRole()->onTax());
            target.setLastAction(ActionType::None);
            return self.getName() + " blocked " + target.getName() + "'s tax.";
        }
        return "Cannot block: Last action was not tax.";
    }



}
