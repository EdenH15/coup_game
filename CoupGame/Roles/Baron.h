//
// Created by eden on 5/8/25.
//

#ifndef BARON_H
#define BARON_H
#include "../Player.h"

namespace CoupG {
    class Baron final : public Player {
    public:
        Baron(Game& game, const std::string& name);
        void useAbility();
        void receiveSanctionBy(Player& p) override;
    };
}


#endif //BARON_H
