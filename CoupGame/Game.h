//
// Created by eden on 5/8/25.
//

#ifndef GAME_H
#define GAME_H
#include <vector>
#include <stdexcept>
#include "Player.h"

namespace CoupG {
    class Game {
    private:
        std::vector<Player*> allPlayers;
        int current_player{};
        bool gameActive{};
        int numPlayers;
        std::string underArrest;
        std::string theWinner{};


    public:
        explicit Game(int numPlayers);
        ~Game();
        void startGame();
        void endGame();
        int getCurrentPlayer() const;
        std::string getUnderArrest() const;
        void setUnderArrest(Player& p);
        void nextTurn();
        std::string getWinner();
        bool Game::isPlayerInGame(const Player* player)const;
        void addPlayer(Player* player);
        void returnPlayer(Player& player) const;
        void removePlayer(Player& player) const;
    };
}

#endif //GAME_H
