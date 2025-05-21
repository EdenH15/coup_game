//
// Created by eden on 5/8/25.
//

#ifndef GOVERNOR_H
#define GOVERNOR_H

#include "../Player.h"
#include <string>

namespace CoupG {
    class Governor final : public Player {
    public:
        Governor(Game& game, const std::string& name);
        void useAbility(Player& p) const;
        void tax() override;
    };
}

#endif //GOVERNOR_H
