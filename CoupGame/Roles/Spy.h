//
// Created by eden on 5/8/25.
//

#ifndef SPY_H
#define SPY_H
#include "Role.h"

namespace CoupG {

    class Spy final : public Role {
    public:
        std::string getName() const override;
        std::string useAbility(Player& self, Player& target) override;
    };
}


#endif //SPY_H
