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

    void Entity::Update(Map::World* world) {
        float mod_y = 0;
        float mod_x = 0;

        if (gravity) {
            Speed_y += GRAVITY;
        }

        mod_y += Speed_y;
        Speed_y = 0;
        bool Col_y = false;
        if (mod_y < 0) {
            if (world->GetCell(round(Pos_y) - 1, Pos_x) == 5 &&
                CheckCollisionRecs(
                    (Rectangle){(float)Pos_y, (float)Pos_x + mod_x, (float)Size_x / TILE_SIZE, (float)Size_y / TILE_SIZE},
                    (Rectangle){(float)Pos_y - 1, (float)Pos_x, (float)TILE_SIZE, (float)TILE_SIZE}
                )) Col_y = true;
        } else if (mod_y > 0) {
            if (world->GetCell(round(Pos_y) + 1, Pos_x) == 5 &&
                CheckCollisionRecs(
                    (Rectangle){(float)Pos_y, (float)Pos_x + mod_x, (float)Size_x / TILE_SIZE, (float)Size_y / TILE_SIZE},
                    (Rectangle){(float)Pos_y + 1, (float)Pos_x, (float)TILE_SIZE, (float)TILE_SIZE}
                )) Col_y = true;
        }
        // for (int y = -1 + Pos_y; y <= 1 + Pos_y; ++y) {
        //     for (int x = -1 + Pos_x; x <= 1 + Pos_x; ++x) {
        //         if (world->GetCell(y, x) == 6) {
        //             if (CheckCollisionRecs(
        //                 (Rectangle){(float)Pos_y + mod_y, (float)Pos_x, (float)Size_x / TILE_SIZE, (float)Size_y / TILE_SIZE},
        //                 (Rectangle){(float)y, (float)x, (float)TILE_SIZE, (float)TILE_SIZE}
        //             )) Col_y = true;
        //         }
        //     }
        // }
        if (!Col_y) {
            std::cout << "Move!" << std::endl;
            Pos_y += mod_y;
        }

        mod_x += Speed_x;
        Speed_x = 0;
        bool Col_x = false;
        if (mod_x < 0) {
            if (world->GetCell(Pos_y, round(Pos_x) - 1) == 5 &&
                CheckCollisionRecs(
                    (Rectangle){(float)Pos_y, (float)Pos_x + mod_x, (float)Size_x / TILE_SIZE, (float)Size_y / TILE_SIZE},
                    (Rectangle){(float)Pos_y, (float)Pos_x - 1, (float)TILE_SIZE, (float)TILE_SIZE}
                )) Col_x = true;
        } else if (mod_x > 0) {
            if (world->GetCell(Pos_y, round(Pos_x) + 1) == 5 &&
                CheckCollisionRecs(
                    (Rectangle){(float)Pos_y, (float)Pos_x + mod_x, (float)Size_x / TILE_SIZE, (float)Size_y / TILE_SIZE},
                    (Rectangle){(float)Pos_y, (float)Pos_x + 1, (float)TILE_SIZE, (float)TILE_SIZE}
                )) Col_x = true;
        }
        // for (int y = -1 + Pos_y; y <= 1 + Pos_y; ++y) {
        //     for (int x = -1 + Pos_x; x <= 1 + Pos_x; ++x) {
        //         if (world->GetCell(y, x) == 6) {
        //             if (CheckCollisionRecs(
        //                 (Rectangle){(float)Pos_y, (float)Pos_x + mod_x, (float)Size_x / TILE_SIZE, (float)Size_y / TILE_SIZE},
        //                 (Rectangle){(float)y, (float)x, (float)TILE_SIZE, (float)TILE_SIZE}
        //             )) Col_x = true;
        //         }
        //     }
        // }
        if (!Col_x) {
            std::cout << "Move!" << std::endl;
            Pos_x += mod_x;
        }
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
