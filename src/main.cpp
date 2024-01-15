#include <iostream>
#include <string>
#include <vector>
#include <raylib.h>
#include "raymath.h"
#include <yaml-cpp/yaml.h>
#include "gfx/gfx.hpp"
#include "world/world.hpp"
#include "world/worldgen.hpp"
#include "entities/entity.hpp"

void camUpdate(Camera2D* camera, Vector2 pos, float delta) {
    static float min_speed = 100;
	static float min_effect_length = 20;
	static float fraction_speed = 1.0f;
	Vector2 v_offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
	camera->offset = v_offset;
	Vector2 diff = Vector2Subtract(pos, camera->target);
	float length = Vector2Length(diff);

	if (length > min_effect_length) {
		float speed = fmaxf(fraction_speed * length, min_speed);
		camera->target = Vector2Add(camera->target, Vector2Scale(diff, speed * delta / 1000 / length));
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

    Vector2 player_position = {
        (float)world->world_size_x * world->Data[0][0]->chunk_size_x / 2,
        (float)world->world_size_y * world->Data[0][0]->chunk_size_y / 2
    };
    Camera2D camera = {};
    camera.target = {0.0f, 0.0f};
    camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Entities::Entity* test_entity = new Entities::Entity(player_position, "./data/assets/test_entity.png", world);

    bool debug_menu = (config["debug"]) ? config["debug"].as<bool>() : false;
    bool smooth_cam = (config["smooth_cam"]) ? config["smooth_cam"].as<bool>() : false;
    float player_speed = (config["player"]["move_speed"]) ? config["player"]["move_speed"].as<float>() / tile_size : 1.0f / tile_size;
    while (!WindowShouldClose()) {
        BeginDrawing();
        float delta = GetFrameTime() * 1000.0f;
        if (IsKeyDown(KEY_W)) test_entity->AddToSpeed((Vector2){0.0f, -player_speed * delta});
        if (IsKeyDown(KEY_S)) test_entity->AddToSpeed((Vector2){0.0f, player_speed * delta});
        if (IsKeyDown(KEY_A)) test_entity->AddToSpeed((Vector2){-player_speed * delta, 0.0f});
        if (IsKeyDown(KEY_D)) test_entity->AddToSpeed((Vector2){player_speed * delta, 0.0f});
        if (IsKeyPressed(KEY_F3)) debug_menu = !debug_menu;
        player_position = test_entity->GetPos();
        test_entity->Update();

        if (smooth_cam) {
            camUpdate(&camera, (Vector2){player_position.x * tile_size, player_position.y * tile_size}, delta);
        } else {
            camera.target = (Vector2){player_position.x * tile_size, player_position.y * tile_size};
        }

        ClearBackground(BLACK);
        BeginMode2D(camera);
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
                        }
                    }
                }
            }
        }

        // Entities
        test_entity->Draw(tile_size);
        EndMode2D();
        EndScissorMode();

        if (debug_menu) {
            DrawFPS(2, 1);

            DrawText((
                "Delta: " +
                std::to_string(delta / 1000) + "ms"
            ).c_str(), 2, 21, 20, DARKGREEN);

            DrawText((
                "Player position: " +
                std::to_string(player_position.x) + "X " +
                std::to_string(player_position.y) + "Y"
            ).c_str(), 2, 41, 20, DARKGREEN);

            Vector2 speed = test_entity->GetSpeed();
            DrawText((
                "Player Speed: " +
                std::to_string(speed.x) + "X " +
                std::to_string(speed.y) + "Y"
            ).c_str(), 2, 61, 20, DARKGREEN);

            DrawText((
                "World size: " +
                std::to_string(world->world_size_x * world->Data[0][0]->chunk_size_x) + "X " +
                std::to_string(world->world_size_y * world->Data[0][0]->chunk_size_y) + "Y"
            ).c_str(), 2, 81, 20, DARKGREEN);
            
            if (smooth_cam) {
                DrawText("Camera smooth mode: TRUE", 0, 101, 20, DARKGREEN);
            } else {
                DrawText("Camera smooth mode: FALSE", 0, 101, 20, DARKGREEN);
            }
        }
        EndDrawing();
    }

    CloseWindow();
}
