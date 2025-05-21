//
// Created by eden on 5/14/25.
//

// PlayerFactory.h
#pragma once
#include "Player.h"
#include "Roles/Baron.h"
#include "Roles/Spy.h"
#include "Roles/General.h"
#include "Roles/Governor.h"
#include "Roles/Merchant.h"
#include "Roles/Judge.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "PlayerFactory.h"

namespace CoupG {

        Player* PlayerFactory::createRandomPlayer(Game& game, const std::string& name) {
            std::srand(std::time(nullptr)); // seed for randomness
            std::vector<std::string> roles = {"BARON", "SPY", "GENERAL", "GOVERNOR", "MERCHANT", "JUDGE"};
            int i = std::rand() % roles.size();
            const std::string& role = roles[i];

            if (role == "BARON") return new Baron(game, name);
            if (role == "SPY") return new Spy(game, name);
            if (role == "GENERAL") return new General(game, name);
            if (role == "GOVERNOR") return new Governor(game, name);
            if (role == "MERCHANT") return new Merchant(game, name);
            if (role == "JUDGE") return new Judge(game, name);

            return nullptr; // fallback
        }
    };

