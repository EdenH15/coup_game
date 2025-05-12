//
// Created by eden on 5/8/25.
//

#ifndef GENERAL_H
#define GENERAL_H

#include "Role.h"

namespace CoupG {
    class General final : public Role {
    public:
        std::string getName() const override;
        std::string useAbility(Player& self, Player& target) override;
        void onArrest(Player& target) override;
    };
}


#endif //GENERAL_H
