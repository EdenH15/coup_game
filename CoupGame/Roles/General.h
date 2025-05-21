//
// Created by eden on 5/8/25.
//

#ifndef GENERAL_H
#define GENERAL_H


#include "../Player.h"
#include <string>

namespace CoupG {
    class General final : public Player {
    public:
        General(Game& game, const std::string& name);
        void useAbility(Player& p);
        void receiveArrestBy(Player& p) override;
    };
}

#endif //GENERAL_H
