#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <vector>
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

class Textures {
    Texture2D texture;
    std::vector<TextureFromAtlas> textures;
    YAML::Node config;

public:
    Textures(std::string Config_path);
    Textures(YAML::Node Config);

    void LoadAll();
    TextureFromAtlas GetTextureI(int index);
    std::vector<TextureFromAtlas> GetTextures();
    Texture2D GetTexture();
};

