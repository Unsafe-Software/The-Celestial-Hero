#include "sprites.hpp"
#include <iostream>
#include <string>

#include <raylib.h>

namespace GFX {
    Sprite::Sprite(std::string path) {
        texture = LoadTexture(path.c_str());
    }

    Sprite::~Sprite() {
        UnloadTexture(texture);
    }

    void Sprite::Draw(int y, int x) {
        Vector2 position = {static_cast<float>(x), static_cast<float>(y)};
        DrawTexture(texture, position.x, position.y, (Color){WHITE});
    }
}
