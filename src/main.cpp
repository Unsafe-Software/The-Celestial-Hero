#include <iostream>
#include <string>
#include <cmath>
#include <raylib.h>

#include "config.hpp"
#include "world/world.hpp"

int main() {
    std::cout << "Booting up " << Config::Name << " " << Config::Version << ";" << std::endl;
    std::cout << Config::License << std::endl << std::endl;

    if (RELEASE == 1) {
        SetTraceLogLevel(LOG_NONE);
    }
    InitWindow(640, 480, "The Celestial Hero");
    SetTargetFPS(60);

    Texture2D texture_black = LoadTexture("./assets/texture_black.png");
    Texture2D texture_white = LoadTexture("./assets/texture_white.png");

    Map::Chunk *chunk = new Map::Chunk();

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
                Vector2 position = {static_cast<float>(x) * 16, static_cast<float>(y) * 16};
                chunk->Data[y][x] == 1 ? DrawTexture(texture_white, position.x, position.y, (Color){WHITE}) :
                    DrawTexture(texture_black, position.x, position.y, (Color){WHITE});
            }
        }
        
        EndDrawing();
    }

    CloseWindow();
}
