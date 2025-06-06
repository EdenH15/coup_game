//
// Created by eden on 5/8/25.
// Email:Edenhassin@gmail.com

#ifndef GAME_H
#define GAME_H
#include <vector>
#include "Player.h"

namespace CoupG {
    class Player;

    class Game {
    private:
        std::vector<Player *> allPlayers;
        size_t current_player{};
        bool gameActive{};
        size_t numPlayers;
        std::string underArrest;
        std::string theWinner;

    public:
        explicit Game(int numPlayers);

        ~Game();

        Game(const Game &) = delete;

        Game &operator=(const Game &) = delete;


        void reset();

        void startGame();

        void endGame();

        void clearPlayers();

        size_t getCurrentPlayer() const;

        std::vector<Player *> getAllPlayers() const;

        size_t getNumPlayers() const;

        std::string getUnderArrest() const;

        std::string getWinner();

        bool isGameActive() const;

        bool isPlayerInGame(const Player *player) const;

        bool isCurrentPlayer(const Player *player) const;

        void setUnderArrest(const std::string &name);

        void setCurrentPlayer(const Player* p);

        void nextTurn();

        std::string turn() const;

        std::vector<std::string> players();

        void addPlayer(Player *player);

        void returnPlayer(Player &player) const;

        void removePlayer(Player &player) const;

        void checkForWinner();

        void validateTurnStart(const Player &p) const;

        void manageNextTurn(Player &p);
    };
}

#endif //GAME_H
