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
            (Pos_x + x) * TILE_SIZE + (SCREEN_WIDTH / 2)
        );
    }
    void Entity::Draw(float y, float x, bool debug) {
        int pos_y = (Pos_y + y) * TILE_SIZE + (SCREEN_HEIGHT / 2);
        int pos_x = (Pos_x + x) * TILE_SIZE + (SCREEN_WIDTH / 2);
        if (pos_y < 0) return;
        if (pos_x < 0) return;
        if (pos_y > SCREEN_HEIGHT) return;
        if (pos_x > SCREEN_WIDTH) return;
        Sprite->Draw(pos_y, pos_x);
        if (debug) {
            DrawRectangleLines(pos_x, pos_y, Size_x, Size_y, GREEN);
            DrawCircle(pos_x, pos_y, 2, RED);
            if (Shape == Circle)
                DrawEllipseLines(pos_x + Size_x / 2, pos_y + Size_y / 2, Size_x / 2, Size_y / 2, BLUE);
        }
    }

    void Entity::Update(Map::World* world) {
        if (gravity && Pos_y < world->world_size_y * world->Data[0][0]->chunk_size_y) {
            if (world->GetCell(Pos_y + 1, Pos_x) != 5) {
                Pos_y += GRAVITY;
            }
        }
    }
}
