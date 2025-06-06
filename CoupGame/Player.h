//
// Created by eden on 5/8/25.
// Email:Edenhassin@gmail.com

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "ActionType.h"
#include "Game.h"



namespace CoupG {
    class Game;
    class Player {
    protected:
        std::string name;
        Game& game;
        int coins;
        bool active;
        std::string role;
        bool underSanction;
        bool blockArrest;
        int anotherTurn=0;
        ActionType lastAction = ActionType::None;



    public:
        Player(Game& game,const std::string& name);
        virtual ~Player() = default;

        //getter
        virtual std::string getName();
        virtual int getCoins() const;
        virtual bool getActive() const;
        virtual int getAnotherTurn() const;
        virtual std::string getRole() const;
        virtual bool isUnderSanction() const;
        virtual ActionType getLastAction() const;
        virtual bool isBlockArrest() const;

        //setter
        virtual void setCoins(int amount);
        virtual void setActive(bool active);
        virtual void setUnderSanction(bool underSanction);
        virtual void setLastAction(ActionType action);
        virtual void setBlockArrest(bool blockArrest);
        virtual void setAnotherTurn(int n);

        // Actions
        virtual void gather();
        virtual int taxAmount() const { return 2; }
        virtual void tax();
        virtual void bribe();
        virtual void arrest(Player& p);
        virtual void sanction(Player& p);
        virtual void coup(Player& p);

        virtual void receiveSanctionBy(Player& p);
        virtual void receiveArrestBy(Player& p);
        virtual void useAbility(){}
        virtual void useAbility(Player& target){(void)target;}
        virtual int useSpyAbility(Player& target){(void)target;return -1;}

    };
}

#endif //PLAYER_H
