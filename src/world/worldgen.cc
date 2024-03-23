#include "worldgen.hh"

namespace Engine {
    namespace World {
        void GenerateWorld(World* world, int seed) {
            int world_size_y = world->world_size_y * world->Data[0][0]->chunk_size_y;
            int world_size_x = world->world_size_x * world->Data[0][0]->chunk_size_x;

            int noise_map[world_size_x];
            PerlinNoise noise(seed);
            for (int x = 0; x < world_size_x; ++x) {
                double nx = static_cast<double>(x) / world_size_x;
                noise_map[x] = static_cast<int>(25 * noise.noise(6 * nx)) + world_size_y / 3;
            }

            std::srand((unsigned)seed);
            for (int x = 0; x < world_size_x; ++x) {
                for (int y = 0; y < world_size_y; ++y) {
                    if (y > noise_map[x]) {
                        world->SetCell(y, x, Stone);
                    }
                }
            }

            for (int y = 0; y < world_size_y; ++y) {
                for (int x = 0; x < world_size_x; ++x) {
                    if (world->GetCell(y, x) == Stone)
                        if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.465) {
                            if (y > noise_map[x])
                                world->SetCell(y, x, Bg);
                            else if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.1)
                                world->SetCell(y, x, Bg);
                        }
                }
            }
            World* world_copy = world;

            for (int t = 0; t < 10; ++t) {
                for (int y = 0; y < world_size_y; ++y) {
                    for (int x = 0; x < world_size_x; ++x) {
                        int noise = noise_map[x];
                        int stone_tiles = 0;
                        int air_tiles = 0;
                        for (int i = -1; i <= 1; ++i) {
                            for (int j = -1; j <= 1; ++j) {
                                if (world_copy->GetCell(y + i, x + j) == Bg)
                                    ++air_tiles;
                                else if (world_copy->GetCell(y + i, x + j) == Stone)
                                    ++stone_tiles;
                            }
                        }
                        if (air_tiles > 4) {
                            if (y > noise) {
                                world->SetCell(y, x, Bg);
                            } else {
                                if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.1) world->SetCell(y, x, Bg);
                            }
                        }
                        if (stone_tiles > 4) {
                            if (y > noise) {
                                world->SetCell(y, x, Stone);
                            } else {
                                if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.1) world->SetCell(y, x, Stone);
                            }
                        }
                    }
                }
                world_copy = world;
            }

            for (int y = 0; y < world_size_y; ++y) {
                for (int x = 0; x < world_size_x; ++x) {
                    if (world->GetCell(y, x) == Stone)
                        if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.43) world->SetCell(y, x, Dirt);
                }
            }
            world_copy = world;

            for (int t = 0; t < 10; ++t) {
                for (int y = 0; y < world_size_y; ++y) {
                    for (int x = 0; x < world_size_x; ++x) {
                        int stone_tiles = 0;
                        int dirt_tiles = 0;
                        for (int i = -1; i <= 1; ++i) {
                            for (int j = -1; j <= 1; ++j) {
                                if (world_copy->GetCell(y + i, x + j) == Dirt)
                                    ++dirt_tiles;
                                else if (world_copy->GetCell(y + i, x + j) == Stone)
                                    ++stone_tiles;
                            }
                        }
                        if (dirt_tiles > 4) world->SetCell(y, x, Dirt);
                        if (stone_tiles > 4) world->SetCell(y, x, Stone);
                    }
                }
                world_copy = world;
            }

            for (int x = 0; x < world_size_x; ++x) {
                int noise = noise_map[x];
                for (int y = 0; y < world_size_y; ++y) {
                    if (y > noise && y < noise + 5) world->SetCell(y, x, Dirt);
                }
            }

            for (int t = 0; t < 10; ++t) {
                for (int y = 0; y < world_size_y; ++y) {
                    for (int x = 0; x < world_size_x; ++x) {
                        int stone_tiles = 0;
                        int dirt_tiles = 0;
                        for (int i = -1; i <= 1; ++i) {
                            for (int j = -1; j <= 1; ++j) {
                                if (world_copy->GetCell(y + i, x + j) == Dirt)
                                    ++dirt_tiles;
                                else if (world_copy->GetCell(y + i, x + j) == Stone)
                                    ++stone_tiles;
                            }
                        }
                        if (dirt_tiles > 4) world->SetCell(y, x, Dirt);
                        if (stone_tiles > 4) world->SetCell(y, x, Stone);
                    }
                }
                world_copy = world;
            }

            bool last_tree_grown = false;
            for (int x = 0; x < world_size_x; ++x) {
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

                for (int y = 0; y < world_size_y; ++y) {
                    if (world->GetCell(y, x) == Dirt) {
                        if (!found_first_block) {
                            found_first_block = true;
                            world->SetCell(y, x, Grass);
                            if (grow_tree) {
                                bool last_grown = false;
                                for (int i = 0; i >= -(std::rand() % 6 + 10); --i) {
                                    world->SetCell(y + i - 1, x, Wood);
                                    if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.2) {
                                        if (!last_grown) {
                                            world->SetCell(y + i - 1, x - 1, Wood);
                                            last_grown = true;
                                        } else {
                                            last_grown = false;
                                        }
                                    } else if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) > 0.8) {
                                        if (!last_grown) {
                                            world->SetCell(y + i - 1, x + 1, Wood);
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
    }  // namespace World
}  // namespace Engine
