#include "LPlayer.h"

LPlayer::LPlayer(int x, int y)
{
    mCollisionBox = {x, y, PLAYER_WIDTH, PLAYER_HEIGHT};
    mVelX = 0;
    mVelY = 0;
    mGravity = 50;
    mJumpHeight = 10;
}
LPlayer::~LPlayer()
{
    mTexture.free();
}
void LPlayer::handleEvent(SDL_Event& e)
{
    if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch(e.key.keysym.sym)
        {
            case SDLK_w: mVelY += mJumpHeight; break;
            case SDLK_a: mVelX -= PLAYER_VEL; break;
            case SDLK_d: mVelX += PLAYER_VEL; break;
        }
    }
    else if(e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        switch(e.key.keysym.sym)
        {
            case SDLK_a: mVelX += PLAYER_VEL; break;
            case SDLK_d: mVelX -= PLAYER_VEL; break;
        }
    }
}
void LPlayer::move(Tile *tiles[], float timeStep)
{
    SDL_Rect tempBox = mCollisionBox;
    mCollisionBox.x += mVelX * timeStep;
    if(mCollisionBox.x < 0)
        mCollisionBox.x = 0;
    else if(mCollisionBox.x > LEVEL_WIDTH - PLAYER_WIDTH)
        mCollisionBox.x = LEVEL_WIDTH - PLAYER_WIDTH;
    mCollisionBox.y += mVelY * timeStep;
    if(mCollisionBox.y < 0)
        mCollisionBox.y = 0;
    else if(mCollisionBox.y > LEVEL_HEIGHT - PLAYER_HEIGHT)
        mCollisionBox.y = LEVEL_HEIGHT - PLAYER_HEIGHT;
    if(touchesWall(mCollisionBox, tiles))
    {
        SDL_Point point = getNearestCollision(mVelX, 0, tempBox, mCollisionBox, tiles);
        mCollisionBox.x = point.x;
        point = getNearestCollision(0, mVelY, tempBox, mCollisionBox, tiles);
        mCollisionBox.y = point.y;
    }
}
void LPlayer::setCamera(SDL_Rect& camera)
{
    camera.x = ((int)mCollisionBox.x + PLAYER_WIDTH / 2) - LOGICAL_SCREEN_WIDTH / 2;
    camera.y = ((int)mCollisionBox.y + PLAYER_HEIGHT / 2) - LOGICAL_SCREEN_HEIGHT / 2;
    camera.w = LOGICAL_SCREEN_WIDTH;
    camera.h = LOGICAL_SCREEN_HEIGHT;
    if(camera.x < 0)
        camera.x = 0;
    if(camera.y < 0)
        camera.y = 0;
    if(camera.x > LEVEL_WIDTH - camera.w)
        camera.x = LEVEL_WIDTH - camera.w;
    if(camera.y > LEVEL_HEIGHT - camera.h)
        camera.y = LEVEL_HEIGHT - camera.h;
}
void LPlayer::render(SDL_Rect& camera)
{
    mTexture.render((int)mCollisionBox.x - camera.x, (int)mCollisionBox.y - camera.y);
}
int LPlayer::getPosX()
{
    return (int)mCollisionBox.x;
}
int LPlayer::getPosY()
{
    return (int)mCollisionBox.y;
}