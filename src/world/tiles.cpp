#include "tiles.hpp"

bool isSolid(Tile tile) {
    for (int i = 0; i < sizeof(Solids) / sizeof(Tile); ++i) {
        if (Solids[i] == tile) {
            return true;
        }
    }
    return false;
}
