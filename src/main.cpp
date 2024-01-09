#include <iostream>
#include <string>
#include <raylib.h>

#include "config.hpp"
#include "world/world.hpp"
#include "gfx/sprites.hpp"

int main() {
    std::cout << "Booting up " << Config::Name << " " << Config::Version << ";" << std::endl;
    std::cout << Config::License << std::endl << std::endl;

    if (RELEASE == 1) {
        SetTraceLogLevel(LOG_NONE);
    }
    InitWindow(640, 480, "The Celestial Hero");
    SetTargetFPS(60);

    GFX::Sprite* sprite_black = new GFX::Sprite("./assets/texture_black.png");
    GFX::Sprite* sprite_white = new GFX::Sprite("./assets/texture_white.png");

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
                chunk->Data[y][x] == 1 ? sprite_white->Draw(y * 16, x * 16) : sprite_black->Draw(y * 16, x * 16);
            }
        }

        DrawFPS(3, 0);
        EndDrawing();
    }

    CloseWindow();
}
