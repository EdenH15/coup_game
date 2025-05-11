//
// Created by eden on 5/8/25.
//

#ifndef GOVERNOR_H
#define GOVERNOR_H

#include "Role.h"
#include <string>

namespace CoupG {
    class Governor final : public Role {
    public:
        std::string getName() const override;
        void useAbility(Player& self, Player& target) override;
        int onTax() override;
    };
}

#endif //GOVERNOR_H
