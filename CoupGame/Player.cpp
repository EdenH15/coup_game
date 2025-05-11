//
// Created by eden on 5/8/25.
//

#include "Player.h"

#include <stdexcept>

#include "Roles/Governor.h"

namespace CoupG {

    Player::Player(const std::string& name, Role* role, const int turnI)
        : name(name), coins(0), active(true), role(role), underSanction(false), turnIndex(turnI) {}

    //getter
    std::string Player::getName() {
        return name;
    }
    int Player::getCoins() const {
        return coins;
    }

    bool Player::getActive() const {
        return active;
    }

    Role* Player::getRole() const {
        return role;
    }

    int Player::getSanctionTurnsLeft() const {
        return sanctionTurnsLeft;
    }

    bool Player::isUnderSanction() const {
        return underSanction;
    }
    ActionType Player::getLastAction() const {
        return lastAction;

    }

    //setter
    void Player::setCoins(const int amount) {
        //what if coins+amount==-num??
        coins+=amount;
    }
    void Player::setActive(const bool active) {
        this->active = active;
    }
    void Player::setSanctionTurnsLeft(int turn) {
        sanctionTurnsLeft=turn;
    }

    void Player::setUnderSanction(const bool underSanction) {
        this->underSanction = underSanction;
    }

    void Player::setLastAction(const ActionType action) {
        this->lastAction = action;
    }

    // Actions
    void Player::gather() {
        coins+=1;
    }
    void Player::tax() {
        coins+=role->onTax();
    }

    void Player::bribe() {
        coins-=4;
    }
    void Player::arrest(Player& p) {
        if (p.getCoins()<1) {
            throw std::runtime_error("Player coins < 1");
        }
        p.setCoins(-1);
        coins+=1;

    }
    void Player::sanction(Player& p) {
        if (coins<3) {
            throw std::runtime_error("Player coins < 3");
        }
        p.setUnderSanction(true);
        p.setSanctionTurnsLeft(1);
        coins-=3;

    }
    void Player::coup(Player& p) {
        if (coins<7) {
            throw std::runtime_error("Player coins < 7");
        }
        coins-=7;
        p.setActive(false);

    }



}
