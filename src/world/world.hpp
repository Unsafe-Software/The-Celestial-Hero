#pragma once

namespace Map {
    class Chunk {
    public:
        const static int chunk_size_x = 16;
        const static int chunk_size_y = 16;

        int Data[chunk_size_y][chunk_size_x];

        Chunk();
        ~Chunk();

        void SetDataCell(int y, int x, int tile);
        void SetDataRow(int y, int tiles[chunk_size_x]);
        void SetDataCol(int x, int tiles[chunk_size_y]);
        void SetData(int tiles[chunk_size_y][chunk_size_x]);

        int GetDataCell(int y, int x);
        int* GetDataRow(int y);
        int* GetDataCol(int x);
        int** GetData();
    };

    class World {
    public:
        const static int world_size_x = 2;
        const static int world_size_y = 2;

        Chunk* Data[world_size_y][world_size_x];

        void SetChunk(int y, int x, Chunk* chunk);
        void SetCell(int y, int x, int data);
        void SetCellByChunk(int chunk_y, int chunk_x, int in_chunk_y, int in_chunk_x, int data);

        Chunk* GetChunk(int y, int x);
        int GetCell(int y, int x);
        int GetCellByChunk(int chunk_y, int chunk_x, int in_chunk_y, int in_chunk_x);
    };
}
