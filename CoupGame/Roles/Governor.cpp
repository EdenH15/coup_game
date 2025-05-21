//
// Created by eden on 5/8/25.
//

#include "Governor.h"
#include <stdexcept>

#include "../Player.h"

namespace CoupG {

    Governor::Governor(Game& game, const std::string& name)
       : Player(game, name) {
        role = "Governor";
    }

    void Governor::tax() {
        coins += 3;
        game.nextTurn();
    }

    void Governor::useAbility(Player &p)  {
        if (!this->active) {
            throw std::runtime_error("Inactive players can't block.");
        }

        if (p.getLastAction() != ActionType::Tax) {
            throw std::runtime_error("Can only block a tax action.");
        }

        p.setCoins(p.getCoins()-2);
    }
}
