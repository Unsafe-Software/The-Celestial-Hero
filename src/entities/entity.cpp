#include "./entity.hpp"
#include <iostream>
#include <raylib.h>
#include "../world/world.hpp"

#define SLOW_DOWN 0.01
#define MAX_SPEED 0.25

namespace Entities {
    void Entity::resolveCollisions() {
        //TODO
    }

    Entity::Entity(Rectangle Bound, Texture2D Texture, Map::World* World) {
        bound = Bound;
        texture = Texture;
        world = World;
    }

    Entity::Entity(Vector2 Position, Texture2D Texture, Map::World* World) {
        texture = Texture;
        bound = (Rectangle){Position.x, Position.y, (float)texture.width, (float)texture.height};
        world = World;
    }


    Entity::Entity(Rectangle Bound, std::string Texture_path, Map::World* World) {
        bound = Bound;
        texture = LoadTexture(Texture_path.c_str());
        world = World;
    }

    Entity::Entity(Vector2 Position, std::string Texture_path, Map::World* World) {
        texture = LoadTexture(Texture_path.c_str());
        bound = (Rectangle){Position.x, Position.y, (float)texture.width, (float)texture.height};
        world = World;
    }

    void Entity::Move(float x, float y) {
        bound.x += x;
        bound.y += y;
        resolveCollisions();
    }

    void Entity::Move(Vector2 Pos) {
        bound.x += Pos.x;
        bound.y += Pos.y;
        resolveCollisions();
    }

    void Entity::Update() {
        bound.x += speed.x;
        bound.y += speed.y;
        if (speed.x > SLOW_DOWN) {
            speed.x -= SLOW_DOWN;
        } else if (speed.x < -SLOW_DOWN) {
            speed.x += SLOW_DOWN;
        } else {
            speed.x = 0;
        }
        if (speed.y > SLOW_DOWN) {
            speed.y -= SLOW_DOWN;
        } else if (speed.y < -SLOW_DOWN) {
            speed.y += SLOW_DOWN;
        } else {
            speed.y = 0;
        }
        if (speed.x > MAX_SPEED) {
            speed.x = MAX_SPEED;
        } else if (speed.x < -MAX_SPEED) {
            speed.x = -MAX_SPEED;
        }
        if (speed.y > MAX_SPEED) {
            speed.y = MAX_SPEED;
        } else if (speed.y < -MAX_SPEED) {
            speed.y = -MAX_SPEED;
        }
        resolveCollisions();
    }

    Rectangle Entity::GetBound() {
        return bound;
    }

    Vector2 Entity::GetPos() {
        return (Vector2){bound.x, bound.y};
    }

    Vector2 Entity::GetSize() {
        return (Vector2){bound.width, bound.height};
    }

    Vector2 Entity::GetSpeed() {
        return speed;
    }

    void Entity::SetBound(Rectangle Buond) {
        bound = Buond;
        resolveCollisions();
    }

    void Entity::SetPos(Vector2 Pos) {
        bound.x = Pos.x;
        bound.y = Pos.y;
        resolveCollisions();
    }

    void Entity::SetSize(Vector2 Size) {
        bound.width = Size.x;
        bound.height = Size.y;
        resolveCollisions();
    }

    void Entity::SetSpeed(Vector2 Speed) {
        speed = Speed;
    }

    void Entity::AddToSpeed(Vector2 AddSpeed) {
        speed.x += AddSpeed.x;
        speed.y += AddSpeed.y;
    }

    void Entity::SetWorld(Map::World* NewWorld, float x, float y) {
        world = NewWorld;
        bound.x = x;
        bound.y = y;
        resolveCollisions();
    }

    void Entity::Draw(int tile_size) {
        DrawTexturePro(
            texture, (Rectangle){0.0f, 0.0f, (float)texture.width, (float)texture.height},
            (Rectangle){bound.x * tile_size, bound.y * tile_size, bound.width, bound.height},
            (Vector2){0, 0}, 0.0f, WHITE
        );
    }
}
