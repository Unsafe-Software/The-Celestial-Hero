#pragma once
#include <cstdlib>
#include "world.hpp"
#include "../gfx/gfx.hpp"
#include "../worldgen_setts.hpp"

namespace Map {
    void GenerateWorldStepOne(World* world, GFX::SpriteList* sprites) {
        int world_size_y_blocks = world->world_size_y * world->Data[0][0]->chunk_size_y;
        int world_size_x_blocks = world->world_size_x * world->Data[0][0]->chunk_size_x;

        for (int y = 0; y < world_size_y_blocks; ++y) {
            for (int x = 0; x < world_size_x_blocks; ++x) {
                if (y >= world_size_y_blocks * DIRT_LEVEL_PERCENT_START / 100) {
                    world->SetCell(y, x, 1);
                }
                if (y >= world_size_y_blocks * STONE_LEVEL_PERCENT_START / 100) {
                    world->SetCell(y, x, 2);
                }
            }
        }
    }

    #define STEP_TWO_STEPS 20
    void GenerateWorldStepTwo(World* world, GFX::SpriteList* sprites, int seed) {
        int world_size_y_blocks = world->world_size_y * world->Data[0][0]->chunk_size_y;
        int world_size_x_blocks = world->world_size_x * world->Data[0][0]->chunk_size_x;
        std::srand(seed);
        World* world_copy = world;

        for (int y = 0; y < world_size_y_blocks; ++y) {
            for (int x = 0; x < world_size_x_blocks; ++x) {
                if (world_copy->GetCell(y, x) == 2)
                    if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.475)
                        world->SetCell(y, x, 3);
            }
        }

        for (int t = 0; t < STEP_TWO_STEPS; ++t) {
            for (int y = 0; y < world_size_y_blocks; ++y) {
                for (int x = 0; x < world_size_x_blocks; ++x) {
                    int stone_tiles = 0;
                    int air_tiles = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            if (world_copy->GetCell(y + i, x + j) == 3) ++air_tiles;
                            else if (world_copy->GetCell(y + i, x + j) == 2) ++stone_tiles;
                        }
                    }
                    if (air_tiles > 4) world->SetCell(y, x, 3);
                    if (stone_tiles > 4) world->SetCell(y, x, 2);
                }
            }
            world_copy = world;
        }
    }

    #define STEP_THREE_STEPS 10
    void GenerateWorldStepThree(World* world, GFX::SpriteList* sprites, int seed) {
        int world_size_y_blocks = world->world_size_y * world->Data[0][0]->chunk_size_y;
        int world_size_x_blocks = world->world_size_x * world->Data[0][0]->chunk_size_x;
        std::srand(seed);
        World* world_copy = world;

        for (int y = 0; y < world_size_y_blocks; ++y) {
            for (int x = 0; x < world_size_x_blocks; ++x) {
                if (world_copy->GetCell(y, x) == 1)
                    if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.475)
                        world->SetCell(y, x, 3);
            }
        }

        for (int t = 0; t < STEP_THREE_STEPS; ++t) {
            for (int y = 0; y < world_size_y_blocks; ++y) {
                for (int x = 0; x < world_size_x_blocks; ++x) {
                    int dirt_tiles = 0;
                    int air_tiles = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            if (world_copy->GetCell(y + i, x + j) == 3) ++air_tiles;
                            else if (world_copy->GetCell(y + i, x + j) == 1) ++dirt_tiles;
                        }
                    }
                    if (air_tiles > 4) world->SetCell(y, x, 3);
                    if (dirt_tiles > 4) world->SetCell(y, x, 1);
                }
            }
            world_copy = world;
        }
    }

    #define STEP_FOUR_STEPS 200
    void GenerateWorldStepFour(World* world, GFX::SpriteList* sprites, int seed) {
        int world_size_y_blocks = world->world_size_y * world->Data[0][0]->chunk_size_y;
        int world_size_x_blocks = world->world_size_x * world->Data[0][0]->chunk_size_x;
        std::srand(seed);
        World* world_copy = world;

        for (int t = 0; t < STEP_FOUR_STEPS; ++t) {
            for (int y = 0; y < world_size_y_blocks; ++y) {
                for (int x = 0; x < world_size_x_blocks; ++x) {
                    if (world_copy->GetCell(y, x) == 1) {
                        int stone_tiles = 0;
                        for (int i = -1; i <= 1; ++i) {
                            for (int j = -1; j <= 1; ++j) {
                                if (world_copy->GetCell(y + i, x + j) == 2) ++stone_tiles;
                            }
                        }
                        if (stone_tiles > 2)
                            if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.05)
                                world->SetCell(y, x, 2);
                    } else if (world_copy->GetCell(y, x) == 2) {
                        int dirt_tiles = 0;
                        for (int i = -1; i <= 1; ++i) {
                            for (int j = -1; j <= 1; ++j) {
                                if (world_copy->GetCell(y + i, x + j) == 1) ++dirt_tiles;
                            }
                        }
                        if (dirt_tiles > 2)
                            if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.05)
                                world->SetCell(y, x, 1);
                    }
                }
            }
            world_copy = world;
        }
    }

    #define STEP_FIVE_STEPS 20
    void GenerateWorldStepFive(World* world, GFX::SpriteList* sprites, int seed) {
        int world_size_y_blocks = world->world_size_y * world->Data[0][0]->chunk_size_y;
        int world_size_x_blocks = world->world_size_x * world->Data[0][0]->chunk_size_x;
        std::srand(seed);
        World* world_copy = world;

        for (int t = 0; t < STEP_FIVE_STEPS; ++t) {
            for (int y = 0; y < world_size_y_blocks; ++y) {
                for (int x = 0; x < world_size_x_blocks; ++x) {
                    int stone_tiles = 0;
                    int dirt_tiles = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            if (world_copy->GetCell(y + i, x + j) == 1) ++dirt_tiles;
                            else if (world_copy->GetCell(y + i, x + j) == 2) ++stone_tiles;
                        }
                    }
                    if (stone_tiles >= 3 && dirt_tiles >= 3) {
                        if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.5)
                            world->SetCell(y, x, 1);
                        else world->SetCell(y, x, 2);
                    }
                }
            }
            world_copy = world;
        }
    }

    #define STEP_SIX_STEPS 5
    void GenerateWorldStepSix(World* world, GFX::SpriteList* sprites, int seed) {
        int world_size_y_blocks = world->world_size_y * world->Data[0][0]->chunk_size_y;
        int world_size_x_blocks = world->world_size_x * world->Data[0][0]->chunk_size_x;
        std::srand(seed);

        for (int y = 0; y < world_size_y_blocks; ++y) {
            for (int x = 0; x < world_size_x_blocks; ++x) {
                if (world->GetCell(y, x) == 2)
                    if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.45) world->SetCell(y, x, 1);
            }
        }
        World* world_copy = world;

        for (int t = 0; t < STEP_SIX_STEPS; ++t) {
            for (int y = 0; y < world_size_y_blocks; ++y) {
                for (int x = 0; x < world_size_x_blocks; ++x) {
                    int stone_tiles = 0;
                    int dirt_tiles = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            if (world_copy->GetCell(y + i, x + j) == 2) ++stone_tiles;
                            if (world_copy->GetCell(y + i, x + j) == 1) ++dirt_tiles;
                        }
                    }
                    if (stone_tiles > 4)
                        world->SetCell(y, x, 2);
                    else if (dirt_tiles > 4)
                        world->SetCell(y, x, 1);
                }
            }
            world_copy = world;
        }
    }

    #define STEP_SEVEN_STEPS 5
    void GenerateWorldStepSeven(World* world, GFX::SpriteList* sprites, int seed) {
        int world_size_y_blocks = world->world_size_y * world->Data[0][0]->chunk_size_y;
        int world_size_x_blocks = world->world_size_x * world->Data[0][0]->chunk_size_x;
        std::srand(seed);

        for (int y = 0; y < world_size_y_blocks; ++y) {
            for (int x = 0; x < world_size_x_blocks; ++x) {
                if (world->GetCell(y, x) == 1)
                    if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.45) world->SetCell(y, x, 2);
            }
        }
        World* world_copy = world;

        for (int t = 0; t < STEP_SEVEN_STEPS; ++t) {
            for (int y = 0; y < world_size_y_blocks; ++y) {
                for (int x = 0; x < world_size_x_blocks; ++x) {
                    int stone_tiles = 0;
                    int dirt_tiles = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            if (world_copy->GetCell(y + i, x + j) == 2) ++stone_tiles;
                            if (world_copy->GetCell(y + i, x + j) == 1) ++dirt_tiles;
                        }
                    }
                    if (stone_tiles > 4)
                        world->SetCell(y, x, 2);
                    else if (dirt_tiles > 4)
                        world->SetCell(y, x, 1);
                }
            }
            world_copy = world;
        }
    }
}
