#include <iostream>
#include <string>
#include <raylib.h>

#include "config.hpp"
#include "settings.conf.h"
#include "world/world.hpp"
#include "world/worldgen.hpp"
#include "gfx/gfx.hpp"
#include "physics/entities.hpp"

void DrawChunk(Map::Chunk* chunk, GFX::SpriteList* sprites, int pos_y, int pos_x) {
    for (int y = 0; y < chunk->chunk_size_y; ++y) {
        for (int x = 0; x < chunk->chunk_size_x; ++x) {
            sprites->Get(chunk->Data[y][x])->Draw(y * TILE_SIZE + pos_y, x * TILE_SIZE + pos_x, 1);
        }
    }
}

int main() {
    std::cout << "Booting up " << NAME << " " << VERSION << ";" << std::endl;
    std::cout << LICENSE << std::endl << std::endl;

    if (RELEASE == 1) {
        SetTraceLogLevel(LOG_WARNING);
    }
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Celestial Hero");
    SetTargetFPS(60);

    GFX::SpriteList* sprites = new GFX::SpriteList("./assets/sprites.txt");
    Map::World* world = new Map::World();
    Vector2 cam_pos = {
        (float)(world->Data[0][0]->chunk_size_x * world->world_size_x * TILE_SIZE) / 2,
        (float)(world->Data[0][0]->chunk_size_y * world->world_size_y * TILE_SIZE) / 2
    };
    int step = 0;
    int seed = 0;
    bool space_pressed = false;
    Map::GenerateWorld(world, sprites);

    Physics::Entity* player = new Physics::Entity(
        world->world_size_y * world->Data[0][0]->chunk_size_y / 2 - 4,
        world->world_size_x * world->Data[0][0]->chunk_size_x / 2 + 2,
        TILE_SIZE,
        TILE_SIZE
    );
    // player->gravity = true;
    player->LoadSprite("./assets/debug.png");

    while (!WindowShouldClose()) {
        BeginDrawing();
        // Updating
        // Drawing
        ClearBackground(BLACK);

        if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) {
            player->Speed_y = PLAYER_SPEED;
        }
        if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) {
            player->Speed_y = -PLAYER_SPEED;
        }
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            player->Speed_x = PLAYER_SPEED;
        }
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) {
            player->Speed_x = -PLAYER_SPEED;
        }

        int world_width = world->world_size_x * world->Data[0][0]->chunk_size_x * TILE_SIZE;
        if (SCREEN_WIDTH > world_width)
            cam_pos.x = SCREEN_WIDTH / 2;
        else if (cam_pos.x < SCREEN_WIDTH / 2) 
            cam_pos.x = SCREEN_WIDTH / 2;
        else if (cam_pos.x > world_width - SCREEN_WIDTH / 2)
            cam_pos.x = world_width - SCREEN_WIDTH / 2;

        int world_height = world->world_size_y * world->Data[0][0]->chunk_size_y * TILE_SIZE;
        if (SCREEN_HEIGHT > world_height)
            cam_pos.y = SCREEN_HEIGHT / 2;
        else if (cam_pos.y < SCREEN_HEIGHT / 2)
            cam_pos.y = SCREEN_HEIGHT / 2;
        else if (cam_pos.y > world_height - SCREEN_HEIGHT / 2)
            cam_pos.y = world_height - SCREEN_HEIGHT / 2;

        int player_y_tiles = cam_pos.y / TILE_SIZE;
        int player_x_tiles = cam_pos.x / TILE_SIZE;
        int clipping_x = (SCREEN_WIDTH / 2) / TILE_SIZE;
        int clipping_y = (SCREEN_HEIGHT / 2) / TILE_SIZE;
        int left_edge_chunk = (player_x_tiles - clipping_x) / world->Data[0][0]->chunk_size_x;
        int rigth_edge_chunk = (player_x_tiles + clipping_y) / world->Data[0][0]->chunk_size_x + 3;
        int up_edge_chunk = (player_y_tiles - clipping_y) / world->Data[0][0]->chunk_size_y;
        int down_edge_chunk = (player_y_tiles + clipping_y) / world->Data[0][0]->chunk_size_y + 1;
        if (left_edge_chunk < 0) left_edge_chunk = 0;
        if (rigth_edge_chunk >= world->world_size_x) rigth_edge_chunk = world->world_size_x;
        if (up_edge_chunk < 0) up_edge_chunk = 0;
        if (down_edge_chunk >= world->world_size_y) down_edge_chunk = world->world_size_y;
        for (int y = up_edge_chunk; y < down_edge_chunk; ++y) {
            for (int x = left_edge_chunk; x < rigth_edge_chunk; ++x) {
                DrawChunk(
                    world->Data[y][x], sprites,
                    y * world->Data[0][0]->chunk_size_y * TILE_SIZE + -cam_pos.y + (int)(SCREEN_HEIGHT / 2),
                    x * world->Data[0][0]->chunk_size_x * TILE_SIZE + -cam_pos.x + (int)(SCREEN_WIDTH / 2)
                );
            }
        }

        DrawFPS(3, 0);
        player->Update(world);
        player->Draw(-cam_pos.y / TILE_SIZE, -cam_pos.x / TILE_SIZE, true);
        EndDrawing();
    }

    CloseWindow();
}
