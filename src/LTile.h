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
        void collisionEvent(int tileNum);
        void updateTiles(float timeStep);
        void render(SDL_Rect& camera);
        void setType(int type);
        int getType();
        SDL_Rect getBox();
    private:
        SDL_Rect mCollisionBox;
        int mType;
        float mActivationTime, mActivationCounter;
};

extern LPlayer* player;
extern LTexture tileTexture;
#endif