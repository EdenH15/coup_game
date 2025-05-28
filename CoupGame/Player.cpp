//
// Created by eden on 5/8/25.
// Email:Edenhassin@gmail.com

#include "Player.h"
#include <iostream>
#include <stdexcept>
#include "Roles/Governor.h"

namespace CoupG {
    /**
     * @brief Constructor for a Player.
     *
     * @param game The game the player is part of.
     * @param name The player's name.
     */
    Player::Player(Game &game, const std::string &name)
        : name(name), game(game), coins(0), active(true), underSanction(false), blockArrest(false) {
    }


    /**
     * @brief Get the player's name.
     * @return Player's name.
     */
    std::string Player::getName() {
        return name;
    }

    /**
     * @brief Get the player's coin count.
     * @return Number of coins the player has.
     */
    int Player::getCoins() const {
        return coins;
    }

    /**
     * @brief Check if the player is active.
     * @return True if active, false otherwise.
     */
    bool Player::getActive() const {
        return active;
    }

    /**
     * @brief Get the player's role.
     * @return The role name.
     */
    std::string Player::getRole() const {
        return role;
    }

    /**
     * @brief Check if the player is under sanction.
     * @return True if under sanction.
     */
    bool Player::isUnderSanction() const {
        return underSanction;
    }

    /**
     * @brief Get the last action performed by the player.
     * @return The last action enum value.
     */
    ActionType Player::getLastAction() const {
        return lastAction;
    }

    /**
     * @brief Check if the player is immune to arrest.
     * @return True if immune.
     */
    bool Player::isBlockArrest() const {
        return blockArrest;
    }

    /**
     * @brief Check if the player has earned another turn.
     * @return True if the player has another turn.
     */
    bool Player::getAnotherTurn() const {
        return anotherTurn;
    }


    /**
     * @brief Set the number of coins.
     * @param amount The amount to set.
     */
    void Player::setCoins(const int amount) {
        coins = amount;
    }

    /**
     * @brief Set whether the player is active.
     * @param active True to set active.
     */
    void Player::setActive(const bool active) {
        this->active = active;
    }

    /**
     * @brief Set whether the player is under sanction.
     * @param underSanction True to sanction the player.
     */
    void Player::setUnderSanction(const bool underSanction) {
        this->underSanction = underSanction;
    }

    /**
     * @brief Set the player's last action.
     * @param action The action performed.
     */
    void Player::setLastAction(const ActionType action) {
        this->lastAction = action;
    }

    /**
     * @brief Set the player's arrest immunity.
     * @param blockArrest True to block arrests.
     */
    void Player::setBlockArrest(const bool blockArrest) {
        this->blockArrest = blockArrest;
    }

    /**
     * @brief Set whether the player gets another turn.
     * @param anotherTurn True to grant another turn.
     */
    void Player::setAnotherTurn(const bool anotherTurn) {
        this->anotherTurn = anotherTurn;
    }

    // === Actions ===

    /**
     * @brief Gather one coin. Fails if under sanction.
     *
     * @throw std::runtime_error if the player is under sanction.
     */
    void Player::gather() {
        game.validateTurnStart(*this);
        if (this->underSanction) {
            throw std::runtime_error("Under sanction");
        }
        coins += 1;
        lastAction = ActionType::Gather;
        game.manageNextTurn(*this);
    }

    /**
     * @brief Take two coins from the treasury. Fails if under sanction.
     *
     * @throw std::runtime_error if the player is under sanction.
     */
    void Player::tax() {
        game.validateTurnStart(*this);
        if (this->underSanction) {
            throw std::runtime_error("Under sanction");
        }
        coins += taxAmount();
        lastAction = ActionType::Tax;
        game.manageNextTurn(*this);
    }


    /**
     * @brief Pay 4 coins to bribe and earn another turn.
     *
     * @throw std::runtime_error if the player has less than 4 coins.
     */
    void Player::bribe() {
        game.validateTurnStart(*this);
        if (coins < 4) {
            throw std::runtime_error("Not enough coins for bribe");
        }
        coins -= 4;
        lastAction = ActionType::Bribe;
        anotherTurn = true;
    }

    /**
     * @brief Attempt to arrest another player.
     * The player steals 1 coin from another player.
     * @param p The player to arrest.
     *
     * @throw std::runtime_error if:
     * - This player is blocked from arrest.
     * - Target has no coins.
     * - Target is already under arrest.
     */
    void Player::arrest(Player &p) {
        game.validateTurnStart(*this);
        if (p.getCoins() < 1) {
            throw std::runtime_error("The player doesn't have enough coins");
        }
        if (this->isBlockArrest()) {
            throw std::runtime_error("Arrest blocked");
        }
        if (p.getName() == game.getUnderArrest()) {
            throw std::runtime_error("Already arrested");
        }
        p.receiveArrestBy(*this);
        game.setUnderArrest(p.getName());
        this->lastAction = ActionType::Arrest;
        game.manageNextTurn(*this);
    }

    /**
     * @brief Sanction another player, costing 3 coins.
     * The sanction blocking them from using GATHER and TAX until their next turn.
     *
     * @param p The player to sanction.
     *
     * @throw std::runtime_error if this player has less than 3 coins.
     */
    void Player::sanction(Player &p) {
        game.validateTurnStart(*this);
        if (coins < 3) {
            throw std::runtime_error("Not enough coins for sanction");
        }
        p.receiveSanctionBy(*this);
        coins -= 3;
        this->lastAction = ActionType::Sanction;
        game.manageNextTurn(*this);
    }

    /**
     * @brief Execute a coup against another player.
     * The player Pays 7 coins to eliminate another player from the game.
     *
     * @param p The player to eliminate.
     *
     * @throw std::runtime_error if the player has less than 7 coins.
     */
    void Player::coup(Player &p) {
        this->lastAction = ActionType::Coup;
        game.validateTurnStart(*this);
        if (coins < 7) {
            throw std::runtime_error("Not enough coins for coup");
        }
        coins -= 7;
        p.setActive(false);
        game.checkForWinner();
        game.manageNextTurn(*this);
    }

    /**
     * @brief Receive a sanction from another player.
     *
     * @param p The player applying the sanction.
     */
    void Player::receiveSanctionBy(Player &p) {
        (void) p;
        this->underSanction = true;
    }

    /**
     * @brief Receive an arrest by another player.
     *
     * Reduces this player's coins by 1 and transfers it to the arresting player.
     *
     * @param p The arresting player.
     */
    void Player::receiveArrestBy(Player &p) {
        this->coins -= 1;
        p.setCoins(p.getCoins() + 1);
    }
}
