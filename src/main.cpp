#include <iostream>
#include <string>
#include <raylib.h>

#include "config.hpp"
#include "world/world.hpp"
#include "gfx/gfx.hpp"

void DrawChunk(Map::Chunk* chunk, GFX::SpriteList* sprites, int pos_y, int pos_x) {
    for (int y = 0; y < chunk->chunk_size_y; ++y) {
        for (int x = 0; x < chunk->chunk_size_x; ++x) {
            sprites->Get(chunk->Data[y][x])->Draw(y * TILE_SIZE + pos_y, x * TILE_SIZE + pos_x);
        }
    }
}

int main() {
    std::cout << "Booting up " << Config::Name << " " << Config::Version << ";" << std::endl;
    std::cout << Config::License << std::endl << std::endl;

    if (RELEASE == 1) {
        SetTraceLogLevel(LOG_WARNING);
    }
    InitWindow(1024, 576, "The Celestial Hero");
    SetTargetFPS(60);

    GFX::SpriteList* sprites = new GFX::SpriteList("./assets/sprites.txt");
    Map::World* world = new Map::World();

    while (!WindowShouldClose()) {
        BeginDrawing();

        for (int y = 0; y < world->world_size_y; ++y) {
            for (int x = 0; x < world->world_size_x; ++x) {
                DrawChunk(
                    world->Data[y][x], sprites,
                    y * world->Data[0][0]->chunk_size_y * TILE_SIZE,
                    x * world->Data[0][0]->chunk_size_x * TILE_SIZE
                );
            }
        }

        DrawFPS(3, 0);
        EndDrawing();
    }

    CloseWindow();
}
