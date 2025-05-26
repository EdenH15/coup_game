//
// Created by eden on 5/8/25.
// Edenhassin@gmail.com

#include "Game.h"
#include "Roles/Merchant.h"
#include <iostream>

namespace CoupG {

    /**
     * @brief Constructor for the Game class.
     * Initializes the game with the specified number of players.
     *
     * @param numPlayers Number of players (must be between 2 and 6).
     * @throws std::invalid_argument if numPlayers is not in [2, 6].
     */
    Game::Game(const int numPlayers) {
        if (numPlayers < 2 || numPlayers > 6) {
            throw std::invalid_argument("Number of players must be between 2 and 6.");
        }
        this->numPlayers = numPlayers;
        startGame();
    }


    void Game::clearPlayers() {
        for (Player *p : allPlayers) {
            delete p;
        }
        allPlayers.clear();
    }

    /**
         * @brief Destructor for the Game class.
         * Frees memory allocated for all players.
         */
    Game::~Game() {
        clearPlayers();
    }

    /**
         * @brief Resets the game state completely.
         * Frees memory for all players and resets game variables.
         */
    void Game::reset() {
        clearPlayers();
        current_player = 0;
        gameActive = false;
        theWinner = "";
        underArrest = "";
    }


    /**
     * @brief Starts a new game by resetting the state.
     */
    void Game::startGame() {
        reset();
        gameActive = true;
    }

    /**
     * @brief Ends the current game.
     */
    void Game::endGame() {
        gameActive = false;
    }

    /**
     * @brief Gets the index of the current player.
     *
     * @return Index of the current player.
     */
    size_t Game::getCurrentPlayer() const {
        return current_player;
    }

    /**
     * @brief Gets the winner of the game.
     *
     * @return Name of the winning player.
     * @throws std::runtime_error if the game is still active.
     */
    std::string Game::getWinner() {
        if (gameActive) {
            throw std::runtime_error("Game is active");
        }
        return theWinner;
    }

    /**
     * @brief Returns the total number of players allowed in the game.
     *
     * @return Number of players.
     */
    size_t Game::getNumPlayers() const {
        return numPlayers;
    }

    /**
     * @brief Checks whether the game is currently active.
     *
     * @return true if the game is active, false otherwise.
     */
    bool Game::isGameActive() const {
        return gameActive;
    }

    /**
     * @brief Checks whether the given player is the current player.
     *
     * @param player Pointer to the player to check.
     * @return true if it's the given player's turn.
     * @throws std::runtime_error if there are no players.
     */
    bool Game::isCurrentPlayer(const Player *player) const {
        if (allPlayers.empty()) {
            throw std::runtime_error("No players in game");
        }
        return allPlayers[current_player] == player;
    }

    /**
     * @brief Gets a vector of all players in the game.
     *
     * @return Vector of pointers to all players.
     */
    std::vector<Player *> Game::getAllPlayers() const {
        return allPlayers;
    }

    /**
     * @brief Gets the name of the player currently under arrest.
     *
     * @return Name of the player under arrest.
     */
    std::string Game::getUnderArrest() const {
        return underArrest;
    }

    /**
     * @brief Sets the name of the player under arrest.
     *
     * @param name Name of the arrested player.
     */
    void Game::setUnderArrest(const std::string &name) {
        underArrest = name;
    }

    /**
     * @brief Advances the turn to the next active player.
     * Also activates Merchant ability if applicable.
     *
     * @throws std::runtime_error if no active players exist.
     */
    void Game::nextTurn() {
        if (allPlayers.empty()) {
            throw std::runtime_error("No players in the game.");
        }
        const size_t count = allPlayers.size();
        for (size_t i = 1; i <= count; ++i) {
            size_t nextIndex = (current_player + i) % count;
            if (allPlayers[nextIndex]->getActive()) {
                current_player = nextIndex;
                Player *p = allPlayers[current_player];
                if (Merchant *merchant = dynamic_cast<Merchant *>(p); merchant != nullptr) {
                    merchant->useAbility();
                }
                return;
            }
        }
        throw std::runtime_error("No active players to switch to.");
    }

    /**
     * @brief Checks if a player exists in the game and is active.
     *
     * @param player Pointer to the player.
     * @return true if the player is in the game and active.
     * @throws std::invalid_argument if the player is not found.
     */
    bool Game::isPlayerInGame(const Player *player) const {
        for (const Player *p: allPlayers) {
            if (p == player) {
                return p->getActive();
            }
        }
        throw std::invalid_argument("Player not found in the game.");
    }

    /**
     * @brief Adds a new player to the game.
     *
     * @param player Pointer to the new player.
     * @throws std::runtime_error if the game already has the maximum number of players.
     */
    void Game::addPlayer(Player *player) {
        if (allPlayers.size() < numPlayers) {
            allPlayers.push_back(player);
        } else {
            throw std::runtime_error("No more players can be added");
        }
    }

    /**
     * @brief Reactivates a previously removed player.
     *
     * @param player Reference to the player.
     * @throws std::runtime_error if the player is not in the game.
     */
    void Game::returnPlayer(Player &player) const {
        if (!isPlayerInGame(&player)) {
            throw std::runtime_error("Player doesn't exist");
        }
        player.setActive(true);
    }

    /**
     * @brief Marks a player as inactive (removes from game).
     *
     * @param player Reference to the player.
     * @throws std::runtime_error if the player is not in the game.
     */
    void Game::removePlayer(Player &player) const {
        if (!isPlayerInGame(&player)) {
            throw std::runtime_error("Player doesn't exist");
        }
        player.setActive(false);
    }

    /**
     * @brief Returns a vector with the names of all active players.
     *
     * @return Vector of active player names.
     */
    std::vector<std::string> Game::players() {
        std::vector<std::string> activePlayers;
        for (Player *p: allPlayers) {
            if (p->getActive()) {
                activePlayers.push_back(p->getName());
            }
        }
        return activePlayers;
    }

    /**
     * @brief Returns the name of the player whose turn it is.
     *
     * @return Name of the current player.
     * @throws std::runtime_error if no active players exist.
     */
    std::string Game::turn() const {
        if (allPlayers.empty()) {
            throw std::runtime_error("No players in the game.");
        }

        size_t index = current_player;

        for (size_t i = 0; i < allPlayers.size(); ++i) {
            Player *p = allPlayers[index];
            if (p->getActive()) {
                return p->getName();
            }
            index = (index + 1) % allPlayers.size();
        }

        throw std::runtime_error("No active players.");
    }

    /**
     * @brief Checks if there's only one active player.
     * If so, ends the game and declares them the winner.
     */
    void Game::checkForWinner() {
        int count = 0;
        std::string lastPlayer;

        for (Player *p: allPlayers) {
            if (p->getActive()) {
                ++count;
                lastPlayer = p->getName();
            }
        }

        if (count == 1) {
            theWinner = lastPlayer;
            endGame();
        }
    }

    /**
     * @brief Validates whether the player can begin their turn.
     *
     * @param p Reference to the player.
     * @throws std::runtime_error if not their turn, player is inactive, or has 10+ coins without using coup.
     */
    void Game::validateTurnStart(const Player &p) const {
        if (!p.getActive()) {
            throw std::runtime_error("Player is not active");
        }
        if (!isCurrentPlayer(&p)) {
            throw std::runtime_error("Not your turn");
        }
        if (p.getCoins() >= 10 && p.getLastAction()!=ActionType::Coup) {
            throw std::runtime_error("Player must perform coup with 10 or more coins");
        }
    }

    /**
     * @brief Handles logic for the end of a player's turn.
     *
     * @param p Reference to the player.
     */
    void Game::manageNextTurn(Player &p) {
        const ActionType action = p.getLastAction();
        if (!p.getAnotherTurn()) {
            if (p.isUnderSanction() && action != ActionType::Tax && action != ActionType::Gather) {
                p.setUnderSanction(false);
            }
            if (p.isBlockArrest() && action != ActionType::Arrest) {
                p.setBlockArrest(false);
            }
            nextTurn();
        } else {
            p.setAnotherTurn(false);
        }
    }

}
