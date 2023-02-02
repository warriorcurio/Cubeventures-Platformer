#ifndef CPROJECTILE_H
#define CPROJECTILE_H
#include "LSDL.h"
#include "CTexture.h"
#include "CTile.h"

class CProjectile
{
    public:
        CProjectile(int x, int y, ProjectileTypes type, int velX, int velY, int respawnX = -1, int respawnY = -1); //Hearts, shields, chargers, energy balls
        CProjectile(int x, int y, int editTileIndex, Tiles editTileNew, bool isOneTimeOnly); //Tile changing buttons
        CProjectile(int x, int y, int editTileIndex, Tiles editTileOriginal, Tiles editTileNew); //Tile toggling buttons
        CProjectile(int x, int y, int teleportX, int teleportY); //Teleporters
        CProjectile(int x, int y, int w, int h, int textX, int textY, const char* textToDisplay, SDL_Color textColour, int size); //Text displayers
        ~CProjectile();
        void move(float timeStep);
        void setPos(int x, int y);
        void render(SDL_Rect& camera);
        int getPosX();
        int getPosY();
        int getType();
        int getEditTileIndex();
        int getEditTileOriginal();
    private:
        void projectileEvent();
        void destroySelf();
        bool touchesTile();
        bool touchesGround();
        bool touchesCeiling();
        SDL_Point getNearestCollision(int xVel, int yVel, SDL_Rect oldBox);
        SDL_Rect mCollisionBox;
        int mUtilityX, mUtilityY;
        int mType;
        int mFrame, mAnimationSpeed;
        int mVelX, mVelY, mGravity;
        int mEditTileIndex, mEditTileOriginal, mEditTileNew;
        bool mDestroyOnPlayerCollision, mDestroyOnTileCollision;
        bool mActivateOnPlayerCollision, mActivateOnTileCollision;
        bool mDisplayText, mHasActivated;
        char mTextToDisplay[50];
        SDL_Color mTextColour;
        int mTextSize;
        CTexture mTextTexture;
};

extern CTexture projectileTexture;
extern std::vector<CProjectile*> projectiles;
#endif