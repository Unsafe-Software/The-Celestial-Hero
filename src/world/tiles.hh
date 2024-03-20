#pragma once

namespace Engine {
    namespace World {
        enum Tile {
            Air,
            Dirt,
            Stone,
            Debug,
            Grass,
            Wood,
            Bg,
            End
        };

        const Tile Solids[] = {
            Dirt,
            Stone,
            Grass
        };

        bool isSolid(Tile tile);
    }
}
