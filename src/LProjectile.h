#ifndef LPROJECTILE_H
#define LPROJECTILE_H
#include "LSDL.h"
#include "LTexture.h"
#include "LTile.h"

class LProjectile
{
    public:
        LProjectile(int x, int y);
        ~LProjectile();
        void handleEvent(SDL_Event* e);
        void move(std::vector<LTile*>& tiles, float timeStep);
        void setCamera(SDL_Rect& camera);
        void checkItemCollisions(std::vector<LTile*>& tiles);
        void setPos(int x, int y);
        void render(SDL_Rect& camera);
        int getPosX();
        int getPosY();
    private:
        bool touchesTile(std::vector<LTile*>& tiles);
        bool touchesGround(std::vector<LTile*>& tiles);
        bool touchesCeiling(std::vector<LTile*>& tiles);
        bool touchesWallRight(std::vector<LTile*>& tiles);
        bool touchesWallLeft(std::vector<LTile*>& tiles);
        SDL_Point getNearestCollision(int xVel, int yVel, SDL_Rect oldBox, std::vector<LTile*>& tiles);
        LTexture mTexture;
        SDL_Rect mCollisionBox;
        SDL_Point mSafePos;
        int mFrame;
        int mPlayerVel, mVelX, mVelY;
        int mGravity, mJumpVelMax, mJumpVelMin, mJumpsRemaining, mMaxJumps;
};
#endif