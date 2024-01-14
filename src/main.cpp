#include <iostream>
#include <string>
#include <raylib.h>
#include "raymath.h"
#include <yaml-cpp/yaml.h>

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

    Camera2D camera = {};
    camera.target = {0.0f, 0.0f};
    camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    Vector2 player_position = {200.0f, 200.0f};

    bool debug_menu = (config["debug"]) ? config["debug"].as<bool>() : false;
    bool smooth_cam = (config["smooth_cam"]) ? config["smooth_cam"].as<bool>() : false;
    while (!WindowShouldClose()) {
        BeginDrawing();
        if (IsKeyDown(KEY_W)) player_position.y -= 5.0f;
        if (IsKeyDown(KEY_S)) player_position.y += 5.0f;
        if (IsKeyDown(KEY_A)) player_position.x -= 5.0f;
        if (IsKeyDown(KEY_D)) player_position.x += 5.0f;
        if (IsKeyPressed(KEY_F3)) debug_menu = !debug_menu;
        float delta = GetFrameTime();

        if (smooth_cam) {
            camUpdate(&camera, player_position, delta);
        } else {
            camera.target = player_position;
        }

        ClearBackground(BLACK);
        BeginMode2D(camera);
        DrawRectangle(100, 100, 20, 20, RED);
        DrawRectangle(player_position.x, player_position.y, 20, 20, BLUE);
        EndMode2D();

        if (debug_menu) {
            DrawText("Debug info:", 0, 0, 20, DARKGREEN);
            DrawFPS(0, 20);
        }
        EndDrawing();
    }

    CloseWindow();
}
