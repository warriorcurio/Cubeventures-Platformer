#ifndef CTILE_H
#define CTILE_H
#include "LSDL.h"
#include "CTexture.h"

class CPlayer;

class CTile
{
    public:
        static const int TILE_WIDTH = 40, TILE_HEIGHT = 40;
        CTile(int x, int y, int tileType);
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

extern bool isDead;
extern CPlayer* player;
extern CTexture tileTexture;
#endif