#include "texture.hpp"
#include <vector>
#include <string>
#include <raylib.h>

TextureFromAtlas::TextureFromAtlas(Rectangle Source) {
    source = Source;
}

TextureFromAtlas::TextureFromAtlas(int Pos_x, int Pos_y, int Size_x, int Size_y) {
    source = (Rectangle){(float)Pos_x, (float)Pos_y, (float)Size_x, (float)Size_y};
}

void TextureFromAtlas::Draw(Texture2D texture, Rectangle Destination, float Rotation) {
    DrawTexturePro(texture, source, Destination, (Vector2){0, 0}, Rotation, WHITE);
}

void TextureFromAtlas::Draw(Texture2D texture, Vector2 Destination, float Scale, float Rotation) {
    DrawTexturePro(
        texture, source,
        (Rectangle){Destination.x, Destination.y, 16, 16},
        (Vector2){0, 0}, Rotation, WHITE
    );
}

void TextureFromAtlas::Draw(Texture2D texture, float Destination_x, float Destination_y, float Scale, float Rotation) {
    DrawTexturePro(
        texture, source,
        (Rectangle){Destination_x, Destination_y, source.width * Scale, source.height * Scale},
        (Vector2){0, 0}, Rotation, WHITE
    );
}

Textures::Textures(std::string Config_path) {
    config = YAML::LoadFile(Config_path);

    std::string texture_atlas_path;
    if (config["texture_atlas"]) {
        texture_atlas_path = config["texture_atlas"].as<std::string>();
    } else {
        std::cerr << "Textures::Textures: Unable to find `texture_atlas` property from yaml provided." << std::endl;
        exit(1);
    }

    texture = LoadTexture(texture_atlas_path.c_str());
}

Textures::Textures(YAML::Node Config) {
    config = Config;

    std::string texture_atlas_path;
    if (config["texture_atlas"]) {
        texture_atlas_path = config["texture_atlas"].as<std::string>();
    } else {
        std::cerr << "Textures::Textures: Unable to find `texture_atlas` property from yaml provided." << std::endl;
        exit(1);
    }

    texture = LoadTexture(texture_atlas_path.c_str());
}

void Textures::LoadAll() {
    YAML::Node textures_data;
    if (config["textures"]) {
        textures_data = config["textures"];
    } else {
        std::cerr << "Textures::LoadAll: Unable to find `textures` property from yaml provided." << std::endl;
        exit(1);
    }

    YAML::Node main_config = YAML::LoadFile("./data/config.yaml");

    for (std::size_t i = 0; i < textures_data.size(); ++i) {
        textures.push_back(TextureFromAtlas(
            (config["textures"][i]["pos_x"]) ? config["textures"][i]["pos_x"].as<int>() : 0,
            (config["textures"][i]["pos_y"]) ? config["textures"][i]["pos_y"].as<int>() : 0,
            (main_config["tile_size"]) ? main_config["tile_size"].as<int>() : 16,
            (main_config["tile_size"]) ? main_config["tile_size"].as<int>() : 16
        ));
    }
}

TextureFromAtlas Textures::GetTextureI(int index) {
    if (index >= 0 && index < textures.size()) {
        return textures[index];
    } else {
        return textures[0];
    }
}

std::vector<TextureFromAtlas> Textures::GetTextures() {
    return textures;
}

Texture2D Textures::GetTexture() {
    return texture;
}
