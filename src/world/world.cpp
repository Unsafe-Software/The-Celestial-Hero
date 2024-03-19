#include "world.hpp"
#include <iostream>

namespace Map {
    Chunk::Chunk() {
        for (int y = 0; y < chunk_size_y; ++y) {
            for (int x = 0; x < chunk_size_x; ++x) {
                Data[y][x] = Air;
            }
        }
    }

    void Chunk::SetDataCell(int y, int x, Tile tile) {
        if (y < 0 || y >= chunk_size_y ||
            x < 0 || x >= chunk_size_x
        ) {
            return;
        }

        Data[y][x] = tile;
    }

    void Chunk::SetDataRow(int y, Tile tiles[chunk_size_x]) {
        if (y < 0 || y >= chunk_size_y) {
            return;
        }

        for (int x = 0; x < chunk_size_x; ++x) {
            Data[y][x] = tiles[x];
        }
    }

    void Chunk::SetDataCol(int x, Tile tiles[chunk_size_y]) {
        if (x < 0 || x >= chunk_size_x) {
            return;
        }

        for (int y = 0; y < chunk_size_y; ++y) {
            Data[y][x] = tiles[y];
        }
    }

    Tile Chunk::GetDataCell(int y, int x) {
        if (y < 0 || y >= chunk_size_y ||
            x < 0 || x >= chunk_size_x
        ) {
            return Debug;
        }

        return Data[y][x];
    }

    Tile* Chunk::GetDataRow(int y) {
        if (y < 0 || y >= chunk_size_y) {
            return {};
        }

        static Tile result_data[chunk_size_x];
        for (int x = 0; x < chunk_size_x; ++x) {
            result_data[x] = Data[y][x];
        }
        return result_data;
    }

    Tile* Chunk::GetDataCol(int x) {
        if (x < 0 || x > chunk_size_x) {
            return {};
        }

        static Tile result_data[chunk_size_y];
        for (int y = 0; y < chunk_size_y; ++y) {
            result_data[y] = Data[y][x];
        }
        return result_data;
    }

    Tile** Chunk::GetData() {
        static Tile** result_data;

        for (int y = 0; y < chunk_size_y; ++y) {
            static Tile* result_data_row;
            for (int x = 0; x < chunk_size_x; ++x) {
                result_data_row[x] = Data[y][x];
            }
            result_data[y] = result_data_row;
        }
        return result_data;
    }

    World::World(int width, int height) {
        world_size_x = width;
        world_size_y = height;
        for (int y = 0; y < height; ++y) {
        std::vector<Chunk*> row;
        for (int x = 0; x < width; ++x) {
            row.push_back(new Chunk());
        }
        Data.push_back(row);
        }
    }

    void World::SetChunk(int y, int x, Chunk* chunk) {
        if (y < 0 || y >= chunk->chunk_size_y ||
            x < 0 || x >= chunk->chunk_size_x) {
        return;
        }

        Data[y][x] = chunk;
    }

    void World::SetCell(int y, int x, Tile data) {
        if (y < 0 || y >= world_size_y * Data[0][0]->chunk_size_y ||
            x < 0 || x >= world_size_x * Data[0][0]->chunk_size_x
        ) {
            return;
        }

        int chunk_y = y / Data[0][0]->chunk_size_y;
        int chunk_x = x / Data[0][0]->chunk_size_x;
        int in_chunk_y = y - chunk_y * Data[0][0]->chunk_size_y;
        int in_chunk_x = x - chunk_x * Data[0][0]->chunk_size_x;

        Data[chunk_y][chunk_x]->Data[in_chunk_y][in_chunk_x] = data;
    }

    void World::SetCellByChunk(int chunk_y, int chunk_x, int in_chunk_y, int in_chunk_x, Tile data) {
        if (in_chunk_y < 0 || in_chunk_y >= Data[0][0]->chunk_size_y ||
            in_chunk_x < 0 || in_chunk_x >= Data[0][0]->chunk_size_x
        ) {
            return;
        }
        if (chunk_y < 0 || chunk_y >= world_size_y ||
            chunk_x < 0 || chunk_x >= world_size_x
        ) {
            return;
        }

        Data[chunk_y][chunk_x]->Data[in_chunk_y][in_chunk_x] = data;
    }

    Tile World::GetCell(int y, int x) {
        if (y < 0 || y >= world_size_y * Data[0][0]->chunk_size_y ||
            x < 0 || x >= world_size_x * Data[0][0]->chunk_size_x
        ) {
            return Debug;
        }

        int chunk_y = y / Data[0][0]->chunk_size_y;
        int chunk_x = x / Data[0][0]->chunk_size_x;
        int in_chunk_y = y - chunk_y * Data[0][0]->chunk_size_y;
        int in_chunk_x = x - chunk_x * Data[0][0]->chunk_size_x;

        return Data[chunk_y][chunk_x]->Data[in_chunk_y][in_chunk_x];
    }

    Tile World::GetCellByChunk(int chunk_y, int chunk_x, int in_chunk_y, int in_chunk_x) {
        if (in_chunk_y < 0 || in_chunk_y >= Data[0][0]->chunk_size_y ||
            in_chunk_x < 0 || in_chunk_x >= Data[0][0]->chunk_size_x
        ) {
            return Data[0][0]->Data[0][0];
        }
        if (chunk_y < 0 || chunk_y >= world_size_y ||
            chunk_x < 0 || chunk_x >= world_size_x
        ) {
            return Data[0][0]->Data[0][0];
        }

        return Data[chunk_y][chunk_x]->Data[in_chunk_y][in_chunk_x];
    }
}
