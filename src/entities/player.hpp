#pragma once

#include <raylib.h>
#include "entity.hpp"

namespace Engine {
    class Player : public Entity {
    private:
        int health;

    public:
        Player(Map::World* world);
        ~Player();

        void Update(bool debug = false) override;
    };
}
