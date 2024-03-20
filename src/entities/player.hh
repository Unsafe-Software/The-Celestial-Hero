#pragma once

#include <raylib.h>

#include "entity.hh"

namespace Engine {
    namespace Entities {
        class Player : public Entity {
        private:
            int health;

        public:
            Player();
            Player(World::World* world);
            ~Player();

            void Update(bool debug = false) override;
        };
    }
}
