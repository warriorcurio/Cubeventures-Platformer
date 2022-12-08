#ifndef LPLAYER_H
#define LPLAYER_H
#include "LSDL.h"
#include "LTexture.h"
#include "LTile.h"

class LPlayer
{
    public:
        static const int PLAYER_WIDTH = 20, PLAYER_HEIGHT = 20;

        LPlayer(int x, int y);
        ~LPlayer();
        void handleEvent(SDL_Event* e);
        void move(std::vector<LTile*>& tiles, float timeStep);
        void setCamera(SDL_Rect& camera);
        void checkItemCollisions(std::vector<LTile*>& tiles);
        void setForm(int form);
        void setHealth(int health);
        void setJumps(int jumps);
        void setKeys(int keys);
        void setPos(int x, int y);
        void setInvulnerable(bool isInvulnerable);
        void render(SDL_Rect& camera);
        int getForm();
        int getHealth();
        SDL_Point getSafePos();
        int getJumps();
        int getKeys();
        int getPosX();
        int getPosY();
        bool getInvulnerable();
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
        int mHealth, mForm, mKeys;
        bool mIsClimbing, mIsInvulnerable, mIsOnGround;
        int mPlayerVel, mVelX, mVelY;
        int mGravity, mJumpVelMax, mJumpVelMin, mJumpsRemaining, mMaxJumps;
};

extern float parallaxOffset;
#endif