//
// Created by eden on 5/8/25.
//

#ifndef JUDGE_H
#define JUDGE_H

#include "Role.h"

namespace CoupG {
    class Judge final : public Role {
    public:
        std::string getName() const override;
        std::string useAbility(Player& self, Player& target) override;
        void onSanction(Player& target) override;
    };
}

#endif //JUDGE_H
