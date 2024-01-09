#include "spritelist.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "sprites.hpp"
#include "../world/world.hpp"

namespace GFX {
    SpriteList::SpriteList(std::string path) {
        std::string line;
        std::ifstream file(path);
        if (file.is_open()) {
            int index = 0;
            while (getline(file, line)) {
                if (!RELEASE) {
                    std::cout << "SpriteList::SpriteList loading texture `" << line << "` with index " << index << "." << std::endl;
                }
                Sprites.push_back(new Sprite("./assets/" + line));
                ++index;
            }
            file.close();
        } else {
            std::cerr << "SpriteList::SpriteList unable to open the file." << std::endl;
            exit(1);
        }
    }
    
    SpriteList::~SpriteList() {
        Sprites.clear();
    }

    Sprite* SpriteList::Get(int index) {
        if (index < 0 || index >= Sprites.size()) {
            std::cerr << "SpriteList::Get index out of range." << std::endl;
            exit(1);
        }

        return Sprites[index];
    }
}