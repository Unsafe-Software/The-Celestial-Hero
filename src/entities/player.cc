#include "player.hh"

namespace Engine {
    namespace Entities {
        Player::Player() : Entity() { this->health = 0; }

        Player::Player(World::World* world) : Entity(world, LoadTexture("./data/assets/entities/player.png")) {
            this->health = 100;
            this->world = world;
        }

        Player::~Player() {}

        void Player::Update(bool debug) { this->updatePosition(debug); }
    }  // namespace Entities
}  // namespace Engine
