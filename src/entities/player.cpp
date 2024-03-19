#include "player.hpp"

namespace Engine {
    Player::Player(Map::World* world) : Entity(world, LoadTexture("./data/assets/player.png")){
        this->health = 100;
        this->world = world;
    }

    Player::~Player() {}

    void Player::Update(bool debug) {
        this->updatePosition(debug);
    }
}
