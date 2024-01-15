#pragma once
#include <string>
#include <raylib.h>
#include "../world/world.hpp"

namespace Entities {
    class Entity {
        Map::World* world;
        Rectangle bound;
        Vector2 speed;
        Texture2D texture;

        void resolveCollisions();

    public:
        Entity(Rectangle Bound, Texture2D Texture, Map::World* World);
        Entity(Vector2 Position, Texture2D Texture, Map::World* World);
        Entity(Rectangle Bound, std::string Texture_path, Map::World* World);
        Entity(Vector2 Position, std::string Texture_path, Map::World* World);

        void Move(float x, float y);
        void Move(Vector2 Pos);
        void Update();
        
        Rectangle GetBound();
        Vector2 GetPos();
        Vector2 GetSize();
        Vector2 GetSpeed();
        void SetBound(Rectangle Buond);
        void SetPos(Vector2 Pos);
        void SetSize(Vector2 Size);
        void SetSpeed(Vector2 Speed);
        void AddToSpeed(Vector2 Speed);
        void SetWorld(Map::World* NewWorld, float x, float y);

        void Draw(int tile_size);
    };
}
