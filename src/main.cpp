#include <iostream>
#include <string>
#include <raylib.h>

#include "config.hpp"
#include "world/world.hpp"
#include "gfx/gfx.hpp"

int main() {
    std::cout << "Booting up " << Config::Name << " " << Config::Version << ";" << std::endl;
    std::cout << Config::License << std::endl << std::endl;

    if (RELEASE == 1) {
        SetTraceLogLevel(LOG_NONE);
    }
    InitWindow(640, 480, "The Celestial Hero");
    SetTargetFPS(60);

    GFX::SpriteList* sprites = new GFX::SpriteList("./assets/sprites.txt");

    Map::Chunk* chunk = new Map::Chunk();
    for (int y = 0; y < chunk->chunk_size_y; ++y) {
        for (int x = 0; x < chunk->chunk_size_x; ++x) {
            std::cout << chunk->Data[y][x] << " ";
        }
        std::cout << std::endl;
    }

    while (!WindowShouldClose()) {
        BeginDrawing();

        for (int y = 0; y < chunk->chunk_size_y; ++y) {
            for (int x = 0; x < chunk->chunk_size_x; ++x) {
                sprites->Get(chunk->Data[y][x])->Draw(y * 16, x * 16);
            }
        }

        DrawFPS(3, 0);
        EndDrawing();
    }

    CloseWindow();
}
