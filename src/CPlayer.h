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
        void move(std::vector<CTile*>& tiles, float timeStep);
        void setCamera(SDL_Rect& camera);
        void checkItemCollisions(std::vector<CTile*>& tiles);
        void setForm(int form);
        void setHealth(int health);
        void setShield(int shield);
        void setJumps(int jumps);
        void setKeys(int keys);
        void setPos(int x, int y);
        void setInvulnerable(bool isInvulnerable);
        void render(SDL_Rect& camera);
        int getForm();
        int getHealth();
        int getShield();
        SDL_Point getSafePos();
        SDL_Rect getBox();
        int getJumps();
        int getKeys();
        int getPosX();
        int getPosY();
        bool getInvulnerable();
    private:
        bool touchesTile(std::vector<CTile*>& tiles);
        bool touchesGround(std::vector<CTile*>& tiles);
        bool touchesCeiling(std::vector<CTile*>& tiles);
        bool touchesWallRight(std::vector<CTile*>& tiles);
        bool touchesWallLeft(std::vector<CTile*>& tiles);
        SDL_Point getNearestCollision(int xVel, int yVel, SDL_Rect oldBox, std::vector<CTile*>& tiles);
        CTexture mTexture;
        SDL_Rect mCollisionBox;
        SDL_Point mSafePos;
        int mFrame;
        int mHealth, mShield, mForm, mKeys;
        bool mIsClimbing, mIsInvulnerable, mIsOnGround;
        int mPlayerVel, mVelX, mVelY;
        int mGravity, mJumpVelMax, mJumpVelMin, mJumpsRemaining, mMaxJumps;
};

extern float parallaxOffset;
#endif