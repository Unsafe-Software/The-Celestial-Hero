#include "game.hh"

#include <iostream>

namespace Engine {
    Game::Game() {
        this->config = YAML::LoadFile("./data/config.yaml");

        if (!this->config["debug"] || this->config["debug"].as<bool>() != true) {
            SetTraceLogLevel(LOG_WARNING);
        }
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow((this->config["startup"]["window"]["width"]) ? this->config["startup"]["window"]["width"].as<int>() : 1280,
            (this->config["startup"]["window"]["height"]) ? this->config["startup"]["window"]["height"].as<int>() : 720,
            (this->config["startup"]["window"]["title_add"])
                ? ("The Celestial Hero: " + this->config["startup"]["window"]["title_add"].as<std::string>()).c_str()
                : "The Celestial Hero");
        SetTargetFPS((this->config["startup"]["window"]["target_fps"]) ? this->config["startup"]["window"]["target_fps"].as<int>() : 60);

        this->blocks = Graphics::Textures("./data/assets/blocks.yaml");
        this->world = World::World((this->config["world"]["width"]) ? this->config["world"]["width"].as<int>() : 10,
            (this->config["world"]["height"]) ? this->config["world"]["height"].as<int>() : 5),
        this->player = Entities::Player(&this->world);

        this->blocks.LoadAll();
        World::GenerateWorld(&this->world);
        this->tile_size = (this->config["tile_size"]) ? this->config["tile_size"].as<int>() : 16;

        this->camera = {};
        this->camera.target = {0.0f, 0.0f};
        this->camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        this->camera.rotation = 0.0f;
        this->camera.zoom = (this->config["zoom"]) ? this->config["zoom"].as<float>() : 1.0f;

        this->player.bounds.x = this->world.world_size_x * this->world.Data[0][0]->chunk_size_x / 2;
        this->player.bounds.y = this->world.world_size_y * this->world.Data[0][0]->chunk_size_y / 2 - 10;
        this->player.bounds.width = 1;
        this->player.bounds.height = 2;

        this->debug = (this->config["debug"]) ? this->config["debug"].as<bool>() : false;
        this->smooth_cam = (this->config["smooth_cam"]) ? this->config["smooth_cam"].as<bool>() : false;
        this->player_speed =
            (this->config["player"]["move_speed"]) ? this->config["player"]["move_speed"].as<float>() / this->tile_size : 1.0f / this->tile_size;
    }

    Game::~Game() {
        delete &this->player;
        delete &this->world;
        delete &this->blocks;
        delete &this->config;
        CloseWindow();
    }

    std::string convert_float_str(float num) {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(3) << num;
        return stream.str();
    }

    void Game::Update() {
        BeginDrawing();
        this->delta = GetFrameTime() * 1000.0f;
        if (IsKeyPressed(KEY_F3)) this->debug = !this->debug;
        if (IsKeyPressed(KEY_F4)) this->smooth_cam = !this->smooth_cam;
        if (IsKeyPressed(KEY_F2)) this->player.noClip = !this->player.noClip;
        if (IsKeyPressed(KEY_F1)) this->draw_hitboxes = !this->draw_hitboxes;
        if (IsKeyDown(KEY_D)) this->player.AddForce({player_speed, 0});
        if (IsKeyDown(KEY_A)) this->player.AddForce({-player_speed, 0});
        if (IsKeyDown(KEY_W)) this->player.AddForce({0, -player_speed});
        if (IsKeyDown(KEY_S)) this->player.AddForce({0, player_speed});

        this->player.Update();

        this->camera_target = (Vector2){this->player.bounds.x * this->tile_size, this->player.bounds.y * this->tile_size};
        this->updateCamera();

        ClearBackground(BLACK);
        BeginMode2D(camera);
        int tiles_drawn = 0;
        for (int y = 0; y < this->world.world_size_y; ++y) {
            for (int x = 0; x < this->world.world_size_x; ++x) {
                if (CheckCollisionRecs(
                        (Rectangle){(float)x * tile_size * this->world.Data[0][0]->chunk_size_x, (float)y * tile_size * this->world.Data[0][0]->chunk_size_y,
                            (float)this->world.Data[0][0]->chunk_size_x * tile_size, (float)this->world.Data[0][0]->chunk_size_y * tile_size},
                        (Rectangle){camera.target.x - GetScreenWidth() / 2, camera.target.y - GetScreenHeight() / 2, (float)GetScreenWidth(),
                            (float)GetScreenHeight()})) {
                    for (int chunk_y = 0; chunk_y < this->world.Data[0][0]->chunk_size_y; ++chunk_y) {
                        for (int chunk_x = 0; chunk_x < this->world.Data[0][0]->chunk_size_x; ++chunk_x) {
                            this->blocks.GetTextureI(this->world.GetCellByChunk(y, x, chunk_y, chunk_x))
                                .Draw(this->blocks.GetTexture(),
                                    (Vector2){((float)x * this->world.Data[0][0]->chunk_size_x + chunk_x) * tile_size,
                                        ((float)y * this->world.Data[0][0]->chunk_size_y + chunk_y) * tile_size},
                                    1.0f, 0.0);
                            ++tiles_drawn;
                        }
                    }
                }
            }
        }

        // Entities
        this->player.Draw(tile_size);
        if (this->draw_hitboxes) {
            // Draw player hitbox
            DrawRectangleLinesEx({this->player.bounds.x * this->tile_size, this->player.bounds.y * this->tile_size, this->player.bounds.width * this->tile_size,
                                     this->player.bounds.height * this->tile_size},
                1, BLUE);
            // Draw tiles hitboxes
            for (Rectangle tile : Entities::GetPossibleCollidingTiles(&this->world, this->player.bounds)) {
                DrawRectangleLinesEx({tile.x, tile.y, tile.width, tile.height}, 1, BLUE);
            }
            // Draw chunk boundaries
            for (int y = 0; y < this->world.world_size_y; ++y) {
                for (int x = 0; x < this->world.world_size_x; ++x) {
                    DrawRectangleLinesEx(
                        {(float)x * tile_size * this->world.Data[0][0]->chunk_size_x, (float)y * tile_size * this->world.Data[0][0]->chunk_size_y,
                            (float)this->world.Data[0][0]->chunk_size_x * tile_size, (float)this->world.Data[0][0]->chunk_size_y * tile_size},
                        1, GREEN);
                }
            }
        }
        EndMode2D();

        if (debug) {
            int pos_y = 2;
            auto color = DARKGRAY;

            DrawText(("FPS: " + std::to_string(GetFPS()) + " (" + convert_float_str(GetFrameTime()) + "ms)").c_str(), 2, pos_y, 20, color);
            pos_y += 20;

            DrawText(("Player position: " + convert_float_str(this->player.bounds.x) + "u " + convert_float_str(this->player.bounds.y) + "u").c_str(), 2, pos_y,
                20, color);
            pos_y += 20;

            DrawText(
                ("Player Velocity: " + convert_float_str(this->player.lastVelocity.x) + "u " + convert_float_str(this->player.lastVelocity.y) + "u").c_str(), 2,
                pos_y, 20, color);
            pos_y += 20;

            DrawText(("Camera target: " + convert_float_str(this->camera_target.x) + "px " + convert_float_str(this->camera_target.y) + "px").c_str(), 2, pos_y,
                20, color);
            pos_y += 20;

            DrawText(("World size: " + std::to_string(this->world.world_size_x) + "ch " + std::to_string(this->world.world_size_y) + "ch || " +
                         std::to_string(this->world.world_size_x * this->world.Data[0][0]->chunk_size_x) + "u " +
                         std::to_string(this->world.world_size_y * this->world.Data[0][0]->chunk_size_y) + "u")
                         .c_str(),
                2, pos_y, 20, color);
            pos_y += 20;

            DrawText(("Tiles drawn: " + std::to_string(tiles_drawn)).c_str(), 2, pos_y, 20, color);
            pos_y += 20;

            pos_y += 40;
            DrawText(("Draw hitboxes (F1): " + static_cast<std::string>(this->draw_hitboxes ? "TRUE" : "FALSE")).c_str(), 2, pos_y, 20, color);
            pos_y += 20;

            DrawText(("No-clip mode (F2): " + static_cast<std::string>(this->player.noClip ? "TRUE" : "FALSE")).c_str(), 2, pos_y, 20, color);
            pos_y += 20;

            DrawText("Debug mode (F3): TRUE", 2, pos_y, 20, color);
            pos_y += 20;

            DrawText(("Smooth camera (F4): " + static_cast<std::string>(this->smooth_cam ? "TRUE" : "FALSE")).c_str(), 2, pos_y, 20, color);
            pos_y += 20;
        }
        EndDrawing();
    }

    void Game::updateCamera() {
        if (smooth_cam) {
            static float min_speed = 10.0f;
            static float min_effect_length = 2.5f;
            static float fraction_speed = 3.0f;
            Vector2 diff = Vector2Subtract(this->camera_target, this->camera.target);
            float length = Vector2Length(diff);

            if (length > min_effect_length) {
                float speed = fmaxf(fraction_speed * length, min_speed);
                this->camera.target = Vector2Add(this->camera.target, Vector2Scale(diff, speed * GetFrameTime() / length));
            }
        } else {
            this->camera.target = this->camera_target;
        }

        // Clamp camera target to map edges if no-clip is off
        if (this->player.noClip == false) {
            if (this->camera.target.x - this->camera.offset.x / this->camera.zoom < 0.0f) this->camera.target.x = this->camera.offset.x / this->camera.zoom;
            if (this->camera.target.y - this->camera.offset.y / this->camera.zoom < 0.0f) this->camera.target.y = this->camera.offset.y / this->camera.zoom;
            if (this->camera.target.x + this->camera.offset.x / this->camera.zoom >
                this->world.world_size_x * this->world.Data[0][0]->chunk_size_x * this->tile_size)
                this->camera.target.x =
                    this->world.world_size_x * this->world.Data[0][0]->chunk_size_x * this->tile_size - this->camera.offset.x / this->camera.zoom;
            if (this->camera.target.y + this->camera.offset.y / this->camera.zoom >
                this->world.world_size_y * this->world.Data[0][0]->chunk_size_y * this->tile_size)
                this->camera.target.y =
                    this->world.world_size_y * this->world.Data[0][0]->chunk_size_y * this->tile_size - this->camera.offset.y / this->camera.zoom;
        }
    }

    bool Game::ShouldClose() { return WindowShouldClose(); }
}  // namespace Engine
