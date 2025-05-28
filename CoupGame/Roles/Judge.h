//
// Created by eden on 5/8/25.
// Email:Edenhassin@gmail.com

#ifndef JUDGE_H
#define JUDGE_H

#include "../Player.h"

namespace CoupG {
    class Judge final : public Player {
    public:
        Judge(Game& game, const std::string& name);

        void useAbility(Player& p) override;
        void receiveSanctionBy(Player& p) override; // נקרא כשהשופט מקבל סנקציה
    };
}
#endif //JUDGE_H
