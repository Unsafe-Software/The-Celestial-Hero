#pragma once
#include <cstdlib>
#include "world.hpp"
#include "../gfx/gfx.hpp"

#define FACTOR_E 0.5
#define SCALE_E 0.3
#define FACTOR_PI -0.8
#define SCALE_PI 0.2
#define FACTOR1 2.8
#define SCALE1 0.1
#define FACTOR_GLOB 3.2
#define E 2.71828

namespace Map {
    void GenerateWorld(World* world) {
        int world_size_y_blocks = world->world_size_y * world->Data[0][0]->chunk_size_y;
        int world_size_x_blocks = world->world_size_x * world->Data[0][0]->chunk_size_x;

        for (int x = 0; x < world_size_x_blocks; ++x) {
            float loc_x = x / 3;
            int noise = FACTOR_GLOB * (FACTOR1 * sin(SCALE1 * loc_x)) - FACTOR_E * sin(SCALE_E * E * loc_x) - FACTOR_PI * sin(SCALE_PI * PI * loc_x) + world_size_y_blocks / 2;
            for (int y = 0; y < world_size_y_blocks; ++y) {
                if (y > noise) {
                    world->SetCell(y, x, 2);
                }
            }
        }

        std::srand(0);
        for (int y = 0; y < world_size_y_blocks; ++y) {
            for (int x = 0; x < world_size_x_blocks; ++x) {
                float loc_x = x / 3;
                int noise = FACTOR_GLOB * (FACTOR1 * sin(SCALE1 * loc_x)) - FACTOR_E * sin(SCALE_E * E * loc_x) - FACTOR_PI * sin(SCALE_PI * PI * loc_x) + world_size_y_blocks / 2 + 10;
                if (world->GetCell(y, x) == 2)
                    if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.465) {
                        if (y > noise) world->SetCell(y, x, 7);
                        else if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.1)
                            world->SetCell(y, x, 7);
                    }
            }
        }
        World* world_copy = world;

        for (int t = 0; t < 10; ++t) {
            for (int y = 0; y < world_size_y_blocks; ++y) {
                for (int x = 0; x < world_size_x_blocks; ++x) {
                    float loc_x = x / 3;
                    int noise = FACTOR_GLOB * (FACTOR1 * sin(SCALE1 * loc_x)) - FACTOR_E * sin(SCALE_E * E * loc_x) - FACTOR_PI * sin(SCALE_PI * PI * loc_x) + world_size_y_blocks / 2 + 10;
                    int stone_tiles = 0;
                    int air_tiles = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            if (world_copy->GetCell(y + i, x + j) == 7) ++air_tiles;
                            else if (world_copy->GetCell(y + i, x + j) == 2) ++stone_tiles;
                        }
                    }
                    if (air_tiles > 4) {
                        if (y > noise) {
                            world->SetCell(y, x, 7);
                        } else {
                            if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.1)
                                world->SetCell(y, x, 7);
                        }
                    }
                    if (stone_tiles > 4) {
                        if (y > noise) {
                            world->SetCell(y, x, 2);
                        } else {
                            if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.1)
                                world->SetCell(y, x, 2);
                        }
                    }
                }
            }
            world_copy = world;
        }

        for (int y = 0; y < world_size_y_blocks; ++y) {
            for (int x = 0; x < world_size_x_blocks; ++x) {
                if (world->GetCell(y, x) == 2)
                    if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.43)
                        world->SetCell(y, x, 1);
            }
        }
        world_copy = world;

        for (int t = 0; t < 10; ++t) {
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
                    if (dirt_tiles > 4) world->SetCell(y, x, 1);
                    if (stone_tiles > 4) world->SetCell(y, x, 2);
                }
            }
            world_copy = world;
        }

        for (int x = 0; x < world_size_x_blocks; ++x) {
            float loc_x = x / 3;
            int noise = FACTOR_GLOB * (FACTOR1 * sin(SCALE1 * loc_x)) - FACTOR_E * sin(SCALE_E * E * loc_x) - FACTOR_PI * sin(SCALE_PI * PI * loc_x) + world_size_y_blocks / 2;
            for (int y = 0; y < world_size_y_blocks; ++y) {
                if (y > noise && y < noise + 5)
                    world->SetCell(y, x, 1);
            }
        }

        for (int t = 0; t < 10; ++t) {
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
                    if (dirt_tiles > 4) world->SetCell(y, x, 1);
                    if (stone_tiles > 4) world->SetCell(y, x, 2);
                }
            }
            world_copy = world;
        }

        bool last_tree_grown = false;
        for (int x = 0; x < world_size_x_blocks; ++x) {
            bool found_first_block = false;
            bool grow_tree = false;
            if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.2) {
                grow_tree = true;
            }
            if (last_tree_grown) {
                grow_tree = false;
                last_tree_grown = false;
            }
            int tree_height = 0;
            bool tree_grown = false;

            for (int y = 0; y < world_size_y_blocks; ++y) {
                if (world->GetCell(y, x) == 1) {
                    if (!found_first_block) {
                        found_first_block = true;
                        world->SetCell(y, x, 5);
                        if (grow_tree) {
                            bool last_grown = false;
                            for (int i = 0; i >= -(std::rand() % 6 + 10); --i) {
                                world->SetCell(y + i - 1, x, 6);
                                if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.2) {
                                    if (!last_grown) {
                                        world->SetCell(y + i - 1, x - 1, 6);
                                        last_grown = true;
                                    } else {
                                        last_grown = false;
                                    }
                                } else if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) > 0.8) {
                                    if (!last_grown) {
                                        world->SetCell(y + i - 1, x + 1, 6);
                                        last_grown = true;
                                    } else {
                                        last_grown = false;
                                    }
                                }
                            }
                            last_tree_grown = true;
                        }
                    }
                }
            }
        }
    }
}
