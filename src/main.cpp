#include <iostream>
#include <string>
#include <cmath>

#include <raylib.h>

#include "config.hpp"

int main() {
    std::cout << "Booting up " << Config::Name << " " << Config::Version << ";" << std::endl;
    std::cout << Config::License << std::endl << std::endl;

    if (RELEASE == 1) {
        SetTraceLogLevel(LOG_NONE);
    }
    InitWindow(640, 480, "The Celestial Hero");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        EndDrawing();
    }

    CloseWindow();
}
