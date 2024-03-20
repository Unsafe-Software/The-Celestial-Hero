#pragma once
#include <vector>

#include "tiles.hh"

namespace Engine {
    namespace World {
        class Chunk {
           public:
            const static int chunk_size_x = 16;
            const static int chunk_size_y = 16;
            Tile Data[chunk_size_y][chunk_size_x];

            Chunk();

            void SetDataCell(int y, int x, Tile tile);
            void SetDataRow(int y, Tile tiles[chunk_size_x]);
            void SetDataCol(int x, Tile tiles[chunk_size_y]);
            void SetData(Tile tiles[chunk_size_y][chunk_size_x]);

            Tile GetDataCell(int y, int x);
            Tile* GetDataRow(int y);
            Tile* GetDataCol(int x);
            Tile** GetData();
        };

        class World {
           public:
            int world_size_x;
            int world_size_y;
            std::vector<std::vector<Chunk*>> Data;

            World();
            World(int width, int height);

            void SetChunk(int y, int x, Chunk* chunk);
            void SetCell(int y, int x, Tile data);
            void SetCellByChunk(int chunk_y, int chunk_x, int in_chunk_y, int in_chunk_x, Tile data);

            Chunk* GetChunk(int y, int x);
            Tile GetCell(int y, int x);
            Tile GetCellByChunk(int chunk_y, int chunk_x, int in_chunk_y, int in_chunk_x);
        };
    }  // namespace World
}  // namespace Engine
