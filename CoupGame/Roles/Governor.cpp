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

    void Governor::useAbility(Player& self, Player& target) {
        if (target.getLastAction()==ActionType::Tax) {
            target.setCoins(-target.getRole()->onTax());
            target.setLastAction(ActionType::None);
        }
        //TROHW
    }



}
