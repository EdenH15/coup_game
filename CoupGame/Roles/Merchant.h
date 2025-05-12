//
// Created by eden on 5/8/25.
//

#ifndef MERCHANT_H
#define MERCHANT_H

#include "Role.h"

namespace CoupG {
    class Merchant final : public Role {
    public:
        std::string getName() const override;
        std::string useAbility(Player& self, Player& target) override;
        void onArrest(Player& target) override;
    };
}



#endif //MERCHANT_H
