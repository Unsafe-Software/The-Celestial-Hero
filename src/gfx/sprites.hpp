#pragma once
#include <iostream>
#include <string>

#include <raylib.h>

namespace GFX {
    class Sprite {
    private:
        Texture2D texture;
    
    public:
        Sprite(std::string path);
        ~Sprite();
        
        void Draw(int y, int x);
    };
}
