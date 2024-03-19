#pragma once
#include <string>
#include <raylib.h>
#include "../world/world.hpp"

namespace Engine {
    class Entity {
    public:
        Rectangle bounds;
        Vector2 velocity;
        Vector2 lastVelocity;
        Map::World* world;
        Texture2D texture;
        bool noClip;

        Entity(Map::World* world, Texture2D texture, bool noClip = false);
        virtual ~Entity();

        virtual void Update(bool debug = false);
        void Draw(int tile_size);

        void AddForce(Vector2 force);
        void updatePosition(bool debug);
        Vector2 ResolveCollisionBox(const Rectangle player, const Vector2 newPlayerPos, const Rectangle box, bool debug);
        void ResolveWorldCollisions(const Vector2 newPlayerPos, bool debug);
    };
}
