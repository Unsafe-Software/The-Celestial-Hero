#pragma once
#include <raylib.h>
#include "../world/world.hpp"

class Entity {
    Map::World* world;
    Rectangle bound;
    Texture2D texture;

    void resolveCollisions();

public:
    Entity(Rectangle Bound, Texture2D Texture, Map::World* World);

    void Move(float x, float y);
    
    Rectangle GetBound();
    Vector2 GetPos();
    Vector2 GetSize();
    void SetBound(Rectangle Buond);
    void SetPos(Vector2 Pos);
    void SetSize(Vector2 Size);
    void SetWorld(Map::World* NewWorld, float x, float y);

    void Draw();
};
