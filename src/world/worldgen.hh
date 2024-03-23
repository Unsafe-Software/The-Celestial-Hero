#pragma once
#include <cstdlib>

#include "../gfx/texture.hh"
#include "perlin_noise.hh"
#include "world.hh"

namespace Engine {
    namespace World {
        void GenerateWorld(World* world, int seed);
    }
}  // namespace Engine
