//
// Created by eden on 5/8/25.
//

#ifndef MERCHANT_H
#define MERCHANT_H

#include "../Player.h"

namespace CoupG {
    class Merchant final : public Player {
    public:
        Merchant(Game& game, const std::string& name);

        void useAbility();
        void receiveArrestBy(Player& p) override;
    };
}


#endif //MERCHANT_H
