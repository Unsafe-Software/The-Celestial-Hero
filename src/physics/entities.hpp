#pragma once
#include <raylib.h>
#include "../gfx/gfx.hpp"

namespace Physics {
    enum ColliderShape {
        Rect,
        Circle,
    };

    class Entity {
    public:
        GFX::Sprite* Sprite;
        float Pos_y;
        float Pos_x;
        int Size_y;
        int Size_x;
        float Speed_y;
        float Speed_x;
        ColliderShape Shape;
        bool gravity;

        Entity();
        Entity(float pos_y, float pos_x);
        Entity(float pos_y, float pos_x, int size_y, int size_x);
        Entity(float pos_y, float pos_x, int size_y, int size_x, ColliderShape shape);

        void Draw(float y, float x);
        void Draw(float y, float x, bool debug);
        void Update(Map::World* world);
        void LoadSprite(std::string path);
        bool CheckEntityCollision(Entity* col);
        // bool CheckWorldCollision(Map::World* world, int y, int x);
    };
}
