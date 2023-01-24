#ifndef CPROJECTILE_H
#define CPROJECTILE_H
#include "LSDL.h"
#include "CTexture.h"
#include "CTile.h"

class CProjectile
{
    public:
        CProjectile(int x, int y, int w, int h, int velX, int velY, int gravity, int type);
        void move(std::vector<CTile*>& tiles, float timeStep);
        void setPos(int x, int y);
        void render(SDL_Rect& camera);
        int getPosX();
        int getPosY();
    private:
        void projectileEvent(std::vector<CTile*>& tiles);
        bool touchesTile(std::vector<CTile*>& tiles);
        bool touchesGround(std::vector<CTile*>& tiles);
        bool touchesCeiling(std::vector<CTile*>& tiles);
        bool touchesWallRight(std::vector<CTile*>& tiles);
        bool touchesWallLeft(std::vector<CTile*>& tiles);
        SDL_Point getNearestCollision(int xVel, int yVel, SDL_Rect oldBox, std::vector<CTile*>& tiles);
        SDL_Rect mCollisionBox;
        int mType;
        int mFrame, mAnimationSpeed;
        int mVelX, mVelY, mGravity;
        bool mDestroyOnPlayerCollision, mDestroyOnTileCollision;
};

extern CTexture projectileTexture;
#endif