//
// Created by eden on 5/8/25.
//

#include "Merchant.h"
#include <iostream>
#include "../Player.h"

namespace CoupG {

    Merchant::Merchant(Game& game, const std::string& name)
    : Player(game, name) {
        this->role = "Merchant";
    }

    void Merchant::useAbility()  {
        if (this->getCoins() >= 3) {
        this->coins+=1;
        std::cout << this->getName() << " starts their turn with 3 or more coins, receiving 1 extra coin!" << std::endl;
    }
        std::cout << this->getName() << "you have less than 3 coins" << std::endl;
    }

    void Merchant::receiveArrestBy(Player& p) {
        (void)p;
        if (coins<1) {
            throw std::runtime_error("2");
        }
        if (this->getCoins() >= 2) {
            this->setCoins(this->coins-2);
            std::cout << this->getName() << " was arrested and paid 2 coins to the bank instead of to another player." << std::endl;
        }
        else {
            this->setCoins(0);
        }
    }
}
