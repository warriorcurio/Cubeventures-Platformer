#ifndef LTILE_H
#define LTILE_H
#include "LSDL.h"
#include "LTexture.h"

class LPlayer;

class LTile
{
    public:
        static const int TILE_WIDTH = 40, TILE_HEIGHT = 40;
        LTile(int x, int y, int tileType);
        void collisionEvent();
        void render(SDL_Rect& camera);
        int getType();
        SDL_Rect getBox();
    private:
        SDL_Rect mCollisionBox;
        int mType;
};

extern LPlayer* player;
extern LTexture tileTexture;
#endif