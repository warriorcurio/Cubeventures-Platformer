#ifndef CPLAYER_H
#define CPLAYER_H
#include "LSDL.h"
#include "CTexture.h"
#include "CTile.h"

class CPlayer
{
    public:
        static const int PLAYER_WIDTH = 20, PLAYER_HEIGHT = 20;

        CPlayer(int x, int y);
        ~CPlayer();
        void handleEvent(SDL_Event* e);
        void move(float timeStep);
        void setCamera(SDL_Rect& camera);
        void checkSpecialTileCollisions();
        void setForm(int form);
        void setCustomForm(int playerVel, int jumpMax, int jumpMin, int gravity);
        void setHealth(int health);
        void setShield(int shield);
        void setCharge(int charge);
        void setJumps(int jumps);
        void setKeys(int keys);
        void setPos(int x, int y);
        void setVel(int velX, int velY);
        void setInvulnerable(bool isInvulnerable);
        void render(SDL_Rect& camera);
        int getForm();
        int getPlayerVel();
        int getJumpMax();
        int getJumpMin();
        int getGravity();
        int getHealth();
        int getShield();
        int getCharge();
        SDL_Point getSafePos();
        SDL_Rect getBox();
        int getJumps();
        int getKeys();
        int getPosX();
        int getPosY();
        int getVelX();
        int getVelY();
        bool getInvulnerable();
    private:
        bool touchesTile();
        bool touchesGround();
        bool touchesCeiling();
        bool touchesWallRight();
        bool touchesWallLeft();
        SDL_Point getNearestCollision(int xVel, int yVel, SDL_Rect oldBox);
        CTexture mTexture;
        SDL_Rect mCollisionBox;
        SDL_Point mSafePos;
        int mFrame;
        int mHealth, mShield, mForm, mKeys, mCharge;
        bool mIsClimbing, mIsInvulnerable, mIsOnGround;
        int mPlayerVel, mVelX, mVelY;
        int mGravity, mJumpVelMax, mJumpVelMin, mJumpsRemaining, mMaxJumps;
};

extern float parallaxOffset;
#endif