#pragma once
#include <string>
#include <vector>

#include "sprites.hpp"
#include "../world/world.hpp"

namespace GFX {
    class SpriteList {
    public:
        std::vector<Sprite*> Sprites;

        SpriteList(std::string path);
        ~SpriteList();
        Sprite* Get(int index);
    };
}
