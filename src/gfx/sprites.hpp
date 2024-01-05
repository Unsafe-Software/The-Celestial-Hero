#pragma once
#include <iostream>
#include <string>

#include <raylib.h>

namespace GFX {
    class SpriteAtlas {
    private:
        Texture2D atlas;
        Sprite* sprites[];
    public:
        SpriteAtlas();
        ~SpriteAtlas();
        int LoadAtlas(std::string path);
        int LoadSprites(std::string path);
    };

    class Sprite {
    private:
        int posY;
        int posX;
        Texture2D* atlasRef;
        int atlasPosY;
        int atlasPosX;
        int atlasSizeY;
        int atlasSizeX;
    public:
        Sprite(Texture2D* AtlasRef);
        Sprite(Texture2D* AtlasRef, int atlasPosY, int atlasPosX, int atlasSizeY, int atlasSizeX);
        ~Sprite();
        void SetBoundaryBox(int atlasPosY, int atlasPosX, int atlasSizeY, int atlasSizeX);
        void SetPosition(int PosY, int PosX);
        void Move(int OffsetY, int OffsetX);
        void Draw();
    };
}
