//
// Created by eden on 5/8/25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "Roles/Role.h"


namespace CoupG {
    class Player {
    private:
        std::string name;
        int coins;
        bool active;
        Role* role;
        bool underSanction;
        int sanctionTurnsLeft = 0;
        int turnIndex;


    public:
        Player(const std::string& name, Role* role);

        //getter
        std::string getName();
        int getCoins();
        bool getActive();
        Role* getRole();
        int getSanctionTurnsLeft();
        bool isUnderSanction();

        //setter
        void setCoins(int amount);
        void setActive(bool active);
        void setSanctionTurnsLeft(int turn);

        // Actions
        void gather();
        void tax();
        void bribe();
        void arrest(Player& p);
        void sanction(Player& target);
        void coup(Player& target);


    };
}

#endif //PLAYER_H
