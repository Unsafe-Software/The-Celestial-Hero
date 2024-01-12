#include "entities.hpp"
#include <raylib.h>
#include <cmath>
#include "../gfx/gfx.hpp"
#include "../settings.conf.h"

namespace Physics {
    Entity::Entity() {
        Sprite = new GFX::Sprite("./assets/debug.png");
        Pos_y = 0;
        Pos_x = 0;
        Size_y = TILE_SIZE;
        Size_x = TILE_SIZE;
        Shape = Rect;
    }
    Entity::Entity(float pos_y, float pos_x) {
        Sprite = new GFX::Sprite("./assets/debug.png");
        Pos_y = pos_y;
        Pos_x = pos_x;
        Size_y = TILE_SIZE;
        Size_x = TILE_SIZE;
        Shape = Rect;
    }
    Entity::Entity(float pos_y, float pos_x, int size_y, int size_x) {
        Sprite = new GFX::Sprite("./assets/debug.png");
        Pos_y = pos_y;
        Pos_x = pos_x;
        Size_y = size_y;
        Size_x = size_x;
        Shape = Rect;
    }
    Entity::Entity(float pos_y, float pos_x, int size_y, int size_x, ColliderShape shape) {
        Sprite = new GFX::Sprite("./assets/debug.png");
        Pos_y = pos_y;
        Pos_x = pos_x;
        Size_y = size_y;
        Size_x = size_x;
        Shape = shape;
    }

    void Entity::Draw(float y, float x) {
        Sprite->Draw(
            (Pos_y + y) * TILE_SIZE + (SCREEN_HEIGHT / 2),
            (Pos_x + x) * TILE_SIZE + (SCREEN_WIDTH / 2),
            Size_x / TILE_SIZE
        );
    }
    void Entity::Draw(float y, float x, bool debug) {
        int pos_y = (Pos_y + y) * TILE_SIZE + (SCREEN_HEIGHT / 2);
        int pos_x = (Pos_x + x) * TILE_SIZE + (SCREEN_WIDTH / 2);
        if (pos_y < 0) return;
        if (pos_x < 0) return;
        if (pos_y > SCREEN_HEIGHT) return;
        if (pos_x > SCREEN_WIDTH) return;
        Sprite->Draw(pos_y, pos_x, Size_x / TILE_SIZE);
        if (debug) {
            DrawRectangleLines(pos_x, pos_y, Size_x, Size_y, GREEN);
            DrawCircle(pos_x, pos_y, 2, RED);
            if (Shape == Circle)
                DrawEllipseLines(pos_x + Size_x / 2, pos_y + Size_y / 2, Size_x / 2, Size_y / 2, BLUE);
        }
    }

    void Entity::Update(Map::World* world, Entity** entities) {
        float mod_y = 0;
        float mod_x = 0;

        if (gravity) {
            // Speed_y += GRAVITY;
        }

        mod_y += Speed_y;
        Speed_y = 0;
        bool Col_y = false;
        for (int i = 0; i < 1; ++i) {
            DrawRectangleRec((Rectangle){Pos_x, mod_y + Pos_y, (float)Size_x / TILE_SIZE, (float)Size_x / TILE_SIZE}, WHITE);
            DrawRectangleRec((Rectangle){entities[i]->Pos_x, entities[i]->Pos_y, (float)entities[i]->Size_x / TILE_SIZE, (float)entities[i]->Size_y / TILE_SIZE}, WHITE);
            if (CheckCollisionRecs(
                (Rectangle){Pos_x, mod_y + Pos_y, (float)Size_x / TILE_SIZE, (float)Size_x / TILE_SIZE},
                (Rectangle){entities[i]->Pos_x, entities[i]->Pos_y, (float)entities[i]->Size_x / TILE_SIZE, (float)entities[i]->Size_x / TILE_SIZE}
            )) Col_y = true;
        }
        if (!Col_y)
            Pos_y += mod_y;

        mod_x += Speed_x;
        Speed_x = 0;
        bool Col_x = false;
        for (int i = 0; i < 1; ++i) {
            DrawRectangleRec((Rectangle){mod_x + Pos_x, Pos_x, (float)Size_x / TILE_SIZE, (float)Size_x / TILE_SIZE}, WHITE);
            DrawRectangleRec((Rectangle){entities[i]->Pos_x, entities[i]->Pos_x, (float)entities[i]->Size_x / TILE_SIZE, (float)entities[i]->Size_x / TILE_SIZE}, WHITE);
            if (CheckCollisionRecs(
                (Rectangle){mod_x + Pos_x, Pos_y, (float)Size_x / TILE_SIZE, (float)Size_y / TILE_SIZE},
                (Rectangle){entities[i]->Pos_x, entities[i]->Pos_y, (float)entities[i]->Size_x / TILE_SIZE, (float)entities[i]->Size_y / TILE_SIZE}
            )) Col_x = true;
        }
        if (!Col_x)
            Pos_x += mod_x;
    }

    void Entity::LoadSprite(std::string path) {
        Sprite = new GFX::Sprite(path);
    }

    bool Check1DCollision(float x11, float x12, float x21, float x22) {
        if (x11 == x12) {
            if (x11 > x21 && x11 <= x22) {
                return true;
            }
        } else {
            if ((x11 > x21 && x11 <= x22) ||
                (x12 > x21 && x12 <= x22)) {
                return true;
            }
        }

        return false;
    }

    bool Entity::CheckEntityCollision(Entity* col) {
        return Check1DCollision(Pos_x, Pos_x + Size_x / TILE_SIZE, col->Pos_x, col->Pos_x + col->Size_x / TILE_SIZE) &&
            Check1DCollision(Pos_y, Pos_y + Size_y / TILE_SIZE, col->Pos_y, col->Pos_y + col->Size_y / TILE_SIZE);
    }

    // bool CheckWorldCollision(Map::World* world, int y, int x) {
    //     return Check1DCollision(Pos_x, Pos_x + Size_x / TILE_SIZE, col->Pos_x, col->Pos_x + col->Size_x / TILE_SIZE) &&
    //         Check1DCollision(Pos_y, Pos_y + Size_y / TILE_SIZE, col->Pos_y, col->Pos_y + col->Size_y / TILE_SIZE);
    // }
}
