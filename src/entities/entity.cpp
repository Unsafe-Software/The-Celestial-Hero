#include "./entity.hpp"
#include <raylib.h>
#include "../world/world.hpp"

void Entity::resolveCollisions() {
    //TODO
}


Entity::Entity(Rectangle Bound, Texture2D Texture, Map::World* World) {
    bound = Bound;
    texture = Texture;
    world = World;
}


void Entity::Move(float x, float y) {
    bound.x += x;
    bound.y += y;
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

void Entity::SetWorld(Map::World* NewWorld, float x, float y) {
    world = NewWorld;
    bound.x = x;
    bound.y = y;
    resolveCollisions();
}

void Entity::Draw() {
    DrawTexturePro(
        texture, (Rectangle){0.0f, 0.0f, texture.width, texture.height},
        bound, (Vector2){0, 0}, 0.0f, WHITE
    );
}
