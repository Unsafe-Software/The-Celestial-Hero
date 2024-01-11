#pragma once
#include <raylib.h>
#include "../gfx/gfx.hpp"

namespace Physics {
    enum ColliderShape {
        Rect,
    }

    class Entity {
    public:
        GFX::Sprite sprite;
        int pos_y;
        int pos_x;
        int size_y;
        int size_x;
        ColliderShape shape;

        Entity(GFX::Sprite* sprite);
        Entity(GFX::Sprite* sprite, int pos_y, int pos_x);
        Entity(GFX::Sprite* sprite, int pos_y, int pos_x, int size_y, int size_x);
        Entity(GFX::Sprite* sprite, int pos_y, int pos_x, int size_y, int size_x, ColliderShape shape);

        Draw();
    };
}
