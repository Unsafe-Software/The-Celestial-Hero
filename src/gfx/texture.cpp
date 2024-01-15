#include "texture.hpp"
#include <vector>
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

std::vector<TextureFromAtlas*> LoadTexturesFromYaml(YAML::Node yaml, YAML::Node config) {
    std::string texture_atlas_path;
    if (yaml["texture_atlas"]) {
        texture_atlas_path = yaml["texture_atlas"].as<std::string>();
    } else {
        std::cerr << "LoadTexturesFromYaml: Unable to find `texture_atlas` property from yaml provided.";
        exit(1);
    }

    YAML::Node textures;
    if (yaml["textures"]) {
        textures = yaml["textures"];
    } else {
        std::cerr << "LoadTexturesFromYaml: Unable to find `textures` property from yaml provided.";
        exit(1);
    }

    std::vector<TextureFromAtlas*> textures_result;
    for (std::size_t i = 0; i < textures.size(); ++i) {
        textures_result.push_back(new TextureFromAtlas(
            (yaml["textures"][i]["pos_x"]) ? yaml["textures"][i]["pos_x"].as<int>() : 0,
            (yaml["textures"][i]["pos_y"]) ? yaml["textures"][i]["pos_y"].as<int>() : 0,
            (config["tile_size"]) ? config["tile_size"].as<int>() : 16,
            (config["tile_size"]) ? config["tile_size"].as<int>() : 16
        ));
    }

    return textures_result;
}
