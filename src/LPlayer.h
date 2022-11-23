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
        void render(SDL_Rect& camera);
        int getForm();
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
        int mForm;
        bool mIsClimbing, mIsOnGround;
        int mPlayerVel, mVelX, mVelY;
        int mGravity, mJumpVelMax, mJumpVelMin, mJumpsRemaining, mMaxJumps;
};
#endif