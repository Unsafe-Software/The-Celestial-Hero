#include <iostream>
#include <string>
#include <vector>
#include <raylib.h>
#include "raymath.h"
#include <yaml-cpp/yaml.h>
#include "gfx/gfx.hpp"
#include "world/world.hpp"
#include "world/worldgen.hpp"

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
		camera->target = Vector2Add(camera->target, Vector2Scale(diff, speed * delta / length));
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

    std::vector<TextureFromAtlas*> blocks = LoadTexturesFromYaml(YAML::LoadFile("./data/assets/blocks.yaml"), config);
    Texture2D blocks_texture = LoadTexture("./data/assets/blocks.png");
    Map::World* world = new Map::World();
    Map::GenerateWorld(world);
    int tile_size = (config["tile_size"]) ? config["tile_size"].as<int>() : 16;

    Vector2 player_position = {
        (float)world->world_size_x * world->Data[0][0]->chunk_size_x * tile_size / 2,
        (float)world->world_size_y * world->Data[0][0]->chunk_size_y * tile_size / 2
    };
    Camera2D camera = {};
    camera.target = {0.0f, 0.0f};
    camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    bool debug_menu = (config["debug"]) ? config["debug"].as<bool>() : false;
    bool smooth_cam = (config["smooth_cam"]) ? config["smooth_cam"].as<bool>() : false;
    float player_speed = (config["player"]["move_speed"]) ? config["player"]["move_speed"].as<int>() : 60;
    while (!WindowShouldClose()) {
        BeginDrawing();
        if (IsKeyDown(KEY_W)) player_position.y -= player_speed;
        if (IsKeyDown(KEY_S)) player_position.y += player_speed;
        if (IsKeyDown(KEY_A)) player_position.x -= player_speed;
        if (IsKeyDown(KEY_D)) player_position.x += player_speed;
        if (IsKeyPressed(KEY_F3)) debug_menu = !debug_menu;
        float delta = GetFrameTime();

        if (smooth_cam) {
            camUpdate(&camera, player_position, delta);
        } else {
            camera.target = player_position;
        }

        ClearBackground(BLACK);
        BeginMode2D(camera);
        for (int y = 0; y < world->world_size_y * world->Data[0][0]->chunk_size_y; ++y) {
            for (int x = 0; x < world->world_size_x * world->Data[0][0]->chunk_size_x; ++x) {
                blocks[world->GetCell(y, x)]->Draw(blocks_texture, (Vector2){(float)x * tile_size, (float)y * tile_size}, 1.0f, 0.0);
            }
        }
        blocks[4]->Draw(blocks_texture, (Vector2){player_position.x, player_position.y}, 1.0f, 0.0f);
        EndMode2D();

        if (debug_menu) {
            DrawFPS(0, 0);

            DrawText((
                "Player position: " +
                std::to_string(player_position.x) + "X " +
                std::to_string(player_position.y) + "Y"
            ).c_str(), 0, 20, 20, DARKGREEN);
            
            if (smooth_cam) {
                DrawText("Camera smooth mode: TRUE", 0, 40, 20, DARKGREEN);
            } else {
                DrawText("Camera smooth mode: FALSE", 0, 40, 20, DARKGREEN);
            }
        }
        EndDrawing();
    }

    CloseWindow();
}
