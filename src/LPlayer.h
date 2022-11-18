#ifndef LPLAYER_H
#define LPLAYER_H
#include "LSDL.h"
#include "LTexture.h"

class LPlayer
{
    public:
        static const int PLAYER_WIDTH = 20, PLAYER_HEIGHT = 20;
        static const int PLAYER_VEL;

        LPlayer(int x, int y);
        ~LPlayer();
        void handleEvent(SDL_Event& e);
        void move(Tile *tiles[], float timeStep);
        void setCamera(SDL_Rect& camera);
        void render(SDL_Rect& camera);
        int getPosX();
        int getPosY();
    private:
        LTexture mTexture;
        SDL_Rect mCollisionBox;
        int mVelX, mVelY;
        int mGravity, mJumpHeight;
};
#endif