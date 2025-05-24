//
// Created by eden on 5/14/25.
//

#ifndef PLAYERFACTORY_H
#define PLAYERFACTORY_H
#pragma once

namespace CoupG {
    class PlayerFactory {
    public:
        static Player* createRandomPlayer(Game& game, const std::string& name);
    };
}

#endif //PLAYERFACTORY_H
