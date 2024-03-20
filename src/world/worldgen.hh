#pragma once
#include <cstdlib>

#include "../gfx/texture.hh"
#include "world.hh"

#define FACTOR_E 0.5
#define SCALE_E 0.3
#define FACTOR_PI -0.8
#define SCALE_PI 0.2
#define FACTOR1 2.8
#define SCALE1 0.1
#define FACTOR_GLOB 3.2
#define E 2.71828

namespace Engine {
    namespace World {
        void GenerateWorld(World* world);
    }
}  // namespace Engine
