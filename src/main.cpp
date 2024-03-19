#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <raylib.h>
#include "raymath.h"
#include <yaml-cpp/yaml.h>
#include "gfx/gfx.hpp"
#include "world/world.hpp"
#include "world/worldgen.hpp"
#include "entities/entity.hpp"
#include "entities/player.hpp"

void camUpdate(Camera2D* camera, Vector2 target, float delta) {
    static float min_speed = 10.0f;
    static float min_effect_length = 2.5f;
    static float fraction_speed = 3.0f;
    Vector2 v_offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera->offset = v_offset;
    Vector2 diff = Vector2Subtract(
        Vector2Multiply(
            target,
            {16.0f, 16.0f}
        ),
        camera->target
    );
    float length = Vector2Length(diff);

    if (length > min_effect_length) {
        float speed = fmaxf(fraction_speed * length, min_speed);
        camera->target = Vector2Add(camera->target, Vector2Scale(diff, speed * GetFrameTime() / length));
    }
}

int main() {
    YAML::Node config = YAML::LoadFile("./data/config.yaml");
    std::cout << "Startup setts:" << std::endl;

    if (!config["debug"] || config["debug"].as<bool>() != true) {
        SetTraceLogLevel(LOG_WARNING);
    }
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(
        (config["startup"]["window"]["width"]) ? config["startup"]["window"]["width"].as<int>() : 1280,
        (config["startup"]["window"]["height"]) ? config["startup"]["window"]["height"].as<int>() : 720,
        (config["startup"]["window"]["title_add"]) ?
            ("The Celestial Hero: " + config["startup"]["window"]["title_add"].as<std::string>()).c_str() :
            "The Celestial Hero"
    );
    SetTargetFPS((config["startup"]["window"]["target_fps"]) ? config["startup"]["window"]["target_fps"].as<int>() : 60);

    Textures* blocks = new Textures("./data/assets/blocks.yaml");
    blocks->LoadAll();
    Map::World* world = new Map::World(
        (config["world"]["width"]) ? config["world"]["width"].as<int>() : 10,
        (config["world"]["height"]) ? config["world"]["height"].as<int>() : 5
    );
    Map::GenerateWorld(world);
    int tile_size = (config["tile_size"]) ? config["tile_size"].as<int>() : 16;

    Camera2D camera = {};
    camera.target = {0.0f, 0.0f};
    camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = (config["zoom"]) ? config["zoom"].as<float>() : 1.0f;

    Engine::Entity* player = new Engine::Player(world);
    player->bounds.x = world->world_size_x * world->Data[0][0]->chunk_size_x / 2;
    player->bounds.y = world->world_size_y * world->Data[0][0]->chunk_size_y / 2 - 10;
    player->bounds.width = 1;
    player->bounds.height = 2;

    bool debug = (config["debug"]) ? config["debug"].as<bool>() : false;
    bool smooth_cam = (config["smooth_cam"]) ? config["smooth_cam"].as<bool>() : false;
    float player_speed = (config["player"]["move_speed"]) ? config["player"]["move_speed"].as<float>() / tile_size : 1.0f / tile_size;
    while (!WindowShouldClose()) {
        BeginDrawing();
        float delta = GetFrameTime() * 1000.0f;
        if (IsKeyPressed(KEY_F3)) debug = !debug;
        if (IsKeyPressed(KEY_F4)) smooth_cam = !smooth_cam;
        if (IsKeyDown(KEY_D)) player->AddForce({player_speed, 0});
        if (IsKeyDown(KEY_A)) player->AddForce({-player_speed, 0});
        if (IsKeyDown(KEY_W)) player->AddForce({0, -player_speed});
        if (IsKeyDown(KEY_S)) player->AddForce({0, player_speed});

        player->Update();

        if (smooth_cam) {
            camUpdate(&camera, (Vector2){player->bounds.x, player->bounds.y}, delta);
        } else {
            camera.target = (Vector2){player->bounds.x * tile_size, player->bounds.y * tile_size};
        }

        ClearBackground(BLACK);
        BeginMode2D(camera);
        int tiles_drawn = 0;
        for (int y = 0; y < world->world_size_y; ++y) {
            for (int x = 0; x < world->world_size_x; ++x) {
                if (CheckCollisionRecs(
                    (Rectangle){
                        (float)x * tile_size * world->Data[0][0]->chunk_size_x,
                        (float)y * tile_size * world->Data[0][0]->chunk_size_y,
                        (float)world->Data[0][0]->chunk_size_x * tile_size,
                        (float)world->Data[0][0]->chunk_size_y * tile_size
                    },
                    (Rectangle){
                        camera.target.x - GetScreenWidth() / 2,
                        camera.target.y - GetScreenHeight() / 2,
                        (float)GetScreenWidth(), (float)GetScreenHeight()
                    }
                )) {
                    for (int chunk_y = 0; chunk_y < world->Data[0][0]->chunk_size_y; ++chunk_y) {
                        for (int chunk_x = 0; chunk_x < world->Data[0][0]->chunk_size_x; ++chunk_x) {
                            blocks->GetTextureI(
                                world->GetCellByChunk(y, x, chunk_y, chunk_x)
                            ).Draw(
                                blocks->GetTexture(),
                                (Vector2){((float)x * world->Data[0][0]->chunk_size_x + chunk_x) * tile_size,
                                ((float)y * world->Data[0][0]->chunk_size_y + chunk_y) * tile_size}, 1.0f, 0.0
                            );
                            ++tiles_drawn;
                        }
                    }
                }
            }
        }

        // Entities
        player->Draw(tile_size);
        EndMode2D();

        if (debug) {
            int pos_y = 2;
            DrawText("Debug mode (F3): TRUE", 2, pos_y, 20, DARKGREEN);
            pos_y += 20;

            std::ostringstream delta_stream;
            delta_stream << std::fixed << std::setprecision(3) << GetFrameTime() * 1000;
            DrawText((
                "FPS: " +
                std::to_string(GetFPS()) +
                " (" + delta_stream.str() + "ms)"
            ).c_str(), 2, pos_y, 20, DARKGREEN);
            pos_y += 20;

            DrawText((
                "Player position: " +
                std::to_string(player->bounds.x) + "u " +
                std::to_string(player->bounds.y) + "u"
            ).c_str(), 2, pos_y, 20, DARKGREEN);
            pos_y += 20;

            DrawText((
                "Cam position: " +
                std::to_string(camera.target.x) + "px " +
                std::to_string(camera.target.y) + "px"
            ).c_str(), 2, pos_y, 20, DARKGREEN);
            pos_y += 20;

            DrawText((
                "Player Velocity: " +
                std::to_string(player->lastVelocity.x) + "u " +
                std::to_string(player->lastVelocity.y) + "u"
            ).c_str(), 2, pos_y, 20, DARKGREEN);
            pos_y += 20;

            DrawText((
                "World size: " +
                std::to_string(world->world_size_x * world->Data[0][0]->chunk_size_x) + "u " +
                std::to_string(world->world_size_y * world->Data[0][0]->chunk_size_y) + "u"
            ).c_str(), 2, pos_y, 20, DARKGREEN);
            pos_y += 20;

            if (smooth_cam) {
                DrawText("Camera smooth mode (F4): TRUE", 0, pos_y, 20, DARKGREEN);
            } else {
                DrawText("Camera smooth mode (F4): FALSE", 0, pos_y, 20, DARKGREEN);
            }
            pos_y += 20;

            DrawText((
                "Tiles drawn: " +
                std::to_string(tiles_drawn)
            ).c_str(), 2, pos_y, 20, DARKGREEN);
            pos_y += 20;
        }
        EndDrawing();
    }

    CloseWindow();
}
