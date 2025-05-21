//
// Created by eden on 5/8/25.
//

#ifndef SPY_H
#define SPY_H
#include "../Player.h"

namespace CoupG {

    class Spy final : public Player {
    public:
        Spy(Game& game, const std::string& name);
        int useAbility(Player& p) const;
    };
}


#endif //SPY_H
