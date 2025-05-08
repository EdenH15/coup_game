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
        int current_player = 0;
        bool gameActive = false;
        int coins = 0;

    public:
        Game(int numPlayers);
        ~Game();
        void startGame();
        void endGame();
        int getCurrentPlayer();
        std::string getWinner();
        int getCoins();
        void addPlayer(Player& player);
        void removePlayer(Player& player);
    };
}

#endif //GAME_H
