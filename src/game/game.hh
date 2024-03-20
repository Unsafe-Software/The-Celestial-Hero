#pragma once
#include <sstream>
#include <iomanip>
#include <raylib.h>
#include <raymath.h>
#include <yaml-cpp/yaml.h>

#include "../gfx/texture.hh"
#include "../world/world.hh"
#include "../world/worldgen.hh"
#include "../entities/entity.hh"
#include "../entities/player.hh"

namespace Engine {
    class Game {
    private:
        YAML::Node config;
        Graphics::Textures blocks;
        World::World world;
        Camera2D camera;
        Entities::Player player;
        Vector2 camera_target;
        bool debug;
        bool smooth_cam;
        int tile_size;
        float player_speed;
        float delta;

        void updateCamera();

    public:
        Game();
        ~Game();
        void Update();

        bool ShouldClose();
    };
}
