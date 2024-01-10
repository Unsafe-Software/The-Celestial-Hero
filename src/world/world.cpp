#include "world.hpp"
#include <iostream>

namespace Map {
  Chunk::Chunk() {
    for (int y = 0; y < chunk_size_y; ++y) {
      for (int x = 0; x < chunk_size_x; ++x) {
        Data[y][x] = 0;
      }
    }
  }

  void Chunk::SetDataCell(int y, int x, int tile) {
    if (y < 0 || y >= chunk_size_y ||
        x < 0 || x >= chunk_size_x) {
      return;
    }

    Data[y][x] = tile;
  }

  void Chunk::SetDataRow(int y, int tiles[chunk_size_x]) {
    if (y < 0 || y >= chunk_size_y) {
      return;
    }

    for (int x = 0; x < chunk_size_x; ++x) {
      Data[y][x] = tiles[x];
    }
  }

  void Chunk::SetDataCol(int x, int tiles[chunk_size_y]) {
    if (x < 0 || x >= chunk_size_x) {
      return;
    }

    for (int y = 0; y < chunk_size_y; ++y) {
      Data[y][x] = tiles[y];
    }
  }

  int Chunk::GetDataCell(int y, int x) {
    if (y < 0 || y >= chunk_size_y ||
        x < 0 || x >= chunk_size_x) {
      return 0;
    }

    return Data[y][x];
  }

  int* Chunk::GetDataRow(int y) {
    if (y < 0 || y >= chunk_size_y) {
      return {};
    }

    static int result_data[chunk_size_x];
    for (int x = 0; x < chunk_size_x; ++x) {
      result_data[x] = Data[y][x];
    }
    return result_data;
  }

  int* Chunk::GetDataCol(int x) {
    if (x < 0 || x > chunk_size_x) {
      return {};
    }

    static int result_data[chunk_size_y];
    for (int y = 0; y < chunk_size_y; ++y) {
      result_data[y] = Data[y][x];
    }
    return result_data;
  }

  int** Chunk::GetData() {
    static int** result_data;

    for (int y = 0; y < chunk_size_y; ++y) {
      static int* result_data_row;
      for (int x = 0; x < chunk_size_x; ++x) {
        result_data_row[x] = Data[y][x];
      }
      result_data[y] = result_data_row;
    }
    return result_data;
  }

  World::World() {
    for (int y = 0; y < world_size_y; ++y) {
      for (int x = 0; x < world_size_x; ++x) {
        Data[y][x] = new Chunk();
      }
    }
  }

  void World::SetChunk(int y, int x, Chunk* chunk) {
    if (y < 0 || y >= chunk->chunk_size_y ||
        x < 0 || x >= chunk->chunk_size_x) {
      return;
    }

    Data[y][x] = chunk;
  }

  void World::SetCell(int y, int x, int data) {
    if (y < 0 || y >= world_size_y * Data[0][0]->chunk_size_y ||
        x < 0 || x >= world_size_x * Data[0][0]->chunk_size_x) {
      return;
    }

    int chunk_y = y / Data[0][0]->chunk_size_y;
    int chunk_x = x / Data[0][0]->chunk_size_x;
    int in_chunk_y = y - chunk_y * Data[0][0]->chunk_size_y;
    int in_chunk_x = x - chunk_x * Data[0][0]->chunk_size_x;

    Data[chunk_y][chunk_x]->Data[in_chunk_y][in_chunk_x] = data;
  }

  void World::SetCellByChunk(int chunk_y, int chunk_x, int in_chunk_y, int in_chunk_x, int data) {
    if (in_chunk_y < 0 || in_chunk_y >= Data[0][0]->chunk_size_y ||
        in_chunk_x < 0 || in_chunk_x >= Data[0][0]->chunk_size_x) {
      return;
    }
    if (chunk_y < 0 || chunk_y >= world_size_y ||
        chunk_x < 0 || chunk_x >= world_size_x) {
      return;
    }

    Data[chunk_y][chunk_x]->Data[in_chunk_y][in_chunk_x] = data;
  }

  int World::GetCell(int y, int x) {
    if (y < 0 || y >= world_size_y * Data[0][0]->chunk_size_y ||
        x < 0 || x >= world_size_x * Data[0][0]->chunk_size_x) {
      return 0;
    }

    int chunk_y = y / Data[0][0]->chunk_size_y;
    int chunk_x = x / Data[0][0]->chunk_size_x;
    int in_chunk_y = y - chunk_y * Data[0][0]->chunk_size_y;
    int in_chunk_x = x - chunk_x * Data[0][0]->chunk_size_x;

    return Data[chunk_y][chunk_x]->Data[in_chunk_y][in_chunk_x];
  }
}
