//
// Created by eden on 5/8/25.
//

#ifndef BARON_H
#define BARON_H
#include "Role.h"

namespace CoupG {
    class Baron final : public Role {
    public:
        std::string getName() const override;
        std::string useAbility(Player& self, Player& target) override;
        void onSanction(Player& target) override;
    };
}


#endif //BARON_H
