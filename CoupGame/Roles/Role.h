//
// Created by eden on 5/10/25.
//

#ifndef ROLE_H
#define ROLE_H

#include <string>


namespace CoupG {
    class Player;
    class Role {
    public:
        virtual ~Role() = default;
        virtual std::string getName() const = 0;
        virtual void useAbility(Player& self, Player& target) = 0;
        virtual int onTax() {return 2;}
        virtual void onBribe(Player& source) {}
        virtual void onArrest(Player& target) {}
        virtual void onCoup(Player& target) {}
    };
}

#endif //ROLE_H
