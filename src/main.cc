#include "game/game.hh"

int main() {
    Engine::Game game;
    while (!game.ShouldClose()) {
        game.Update();
    }
    return 0;
}
