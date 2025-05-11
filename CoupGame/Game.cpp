//
// Created by eden on 5/8/25.
//

#include "Game.h"

namespace CoupG {
    Game::Game(const int numPlayers): numPlayers(numPlayers){}

    void Game::startGame() {
        current_player = 0;
        gameActive = true;
    }

    void Game::endGame() {
        gameActive = false;
    }

    int Game::getCurrentPlayer() const {
        return current_player;
    }

    void Game::nextTurn() {
        current_player = (current_player + 1) % numPlayers;
    }

    std::string Game::getWinner() {
        return theWinner;
    }

    bool Game::isPlayerInGame(const Player* player) const {
        for (const Player* p : allPlayers) {
            if (p == player) {
                return true;
            }
        }
        return false;
    }

    void Game::addPlayer(Player* player){
        if (allPlayers.size()<numPlayers) {
            allPlayers.push_back(player);
        }
        else {
            throw std::runtime_error("No more players can be added");
        }
    }


    void Game::returnPlayer(Player& player) const {
        if (!isPlayerInGame(&player)) {
            throw std::runtime_error("Player doesn't exist");
        }
        player.setActive(true);
    }

    void Game::removePlayer(Player& player) const {
        if (!isPlayerInGame(&player)) {
            throw std::runtime_error("Player doesn't exist");
        }
        player.setActive(false);
    }

    std::string Game::getUnderArrest() const {
        return underArrest;
    }

    void Game::setUnderArrest(Player& p) {
        underArrest = p.getName();

    }


}
