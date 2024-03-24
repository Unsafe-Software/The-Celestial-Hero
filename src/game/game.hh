#pragma once
#include <glog/logging.h>
#include <raylib.h>
#include <raymath.h>
#include <yaml-cpp/yaml.h>

#include <iomanip>
#include <sstream>

#include "../entities/entity.hh"
#include "../entities/player.hh"
#include "../gfx/texture.hh"
#include "../saves/saves.hh"
#include "../world/world.hh"
#include "../world/worldgen.hh"

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
        bool draw_hitboxes;
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
}  // namespace Engine
