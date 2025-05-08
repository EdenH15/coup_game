//
// Created by eden on 5/8/25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <string>


namespace CoupG {
    class Player {
    private:
        std::string name;
        int coins;
        bool active;
        Role role;
        bool underSanction;
        int sanctionTurnsLeft = 0;


    public:
        Player(const std::string& name, Role role);
        std::string getName();
        int getCoins();
        void setCoins(int coins);
        bool getActive();
        void setActive(bool active);
        Role getRole();

        void gather();
        void tax();
        void bribe();
        void arrest(Player& p);
        void sanction(Player& target);
        void coup(Player& target);


    };
}

#endif //PLAYER_H
