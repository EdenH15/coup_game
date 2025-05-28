//
// Created by eden on 5/8/25.
// Email:Edenhassin@gmail.com

#include "Governor.h"
#include <stdexcept>

#include "../Player.h"

namespace CoupG {
    Governor::Governor(Game &game, const std::string &name)
        : Player(game, name) {
        role = "Governor";
    }

    void Governor::useAbility(Player &p) {
        if (!this->active) {
            throw std::runtime_error("Inactive players can't block.");
        }
        if (p.getLastAction() != ActionType::Tax) {
            throw std::runtime_error("Can only block a tax action.");
        }
        if (p.getRole() == "Governor") {
            p.setCoins(p.getCoins() - 3);
        } else {
            p.setCoins(p.getCoins() - 2);
        }
        lastAction = ActionType::UndoTax;
    }
}
