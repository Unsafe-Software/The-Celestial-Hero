#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <raylib.h>
#include <yaml-cpp/yaml.h>

class TextureFromAtlas {
    Rectangle source;
    
public:
    TextureFromAtlas(Rectangle Source);
    TextureFromAtlas(int Pos_x, int Pos_y, int Size_x, int Size_y);
    void Draw(Texture2D texture, Rectangle Destination, float Rotation);
    void Draw(Texture2D texture, Vector2 Destination, float Scale, float Rotation);
    void Draw(Texture2D texture, float Destination_x, float Destination_y, float Scale, float Rotation);
};

std::vector<TextureFromAtlas*> LoadTexturesFromYaml(YAML::Node yaml, YAML::Node config);
