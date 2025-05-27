//
// Created by eden on 5/14/25.
//

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
#include <random>
#include <vector>
#include <string>
#include <chrono>

namespace CoupG {

    Player* PlayerFactory::createRandomPlayer(Game &game, const std::string &name) {
        static std::default_random_engine engine{
            static_cast<unsigned int>(
                std::chrono::system_clock::now().time_since_epoch().count()
            )
        };

        const std::vector<std::string> roles = {"BARON", "SPY", "GENERAL", "GOVERNOR", "MERCHANT", "JUDGE"};
        std::uniform_int_distribution<int> dist(0, roles.size() - 1);
        int i = dist(engine);

        const std::string &role = roles[i];

        if (role == "BARON") return new Baron(game, name);
        if (role == "SPY") return new Spy(game, name);
        if (role == "GENERAL") return new General(game, name);
        if (role == "GOVERNOR") return new Governor(game, name);
        if (role == "MERCHANT") return new Merchant(game, name);
        if (role == "JUDGE") return new Judge(game, name);

        return nullptr;
    }
};
