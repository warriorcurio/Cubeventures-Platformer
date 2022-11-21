#include "LPlayer.h"

LPlayer::LPlayer(int x, int y)
{
    mTexture.loadFromFile("res/player.png");
    mCollisionBox = {x, y, PLAYER_WIDTH, PLAYER_HEIGHT};
    mPlayerVel = 300;
    mVelX = 0;
    mVelY = 0;
    mGravity = 1680;
    mJumpVelMax = 840;
    mJumpVelMin = 380;
    mJumpsRemaining = 1;
}
LPlayer::~LPlayer()
{
    mTexture.free();
}
void LPlayer::handleEvent(SDL_Event* e)
{
    if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
        switch (e->key.keysym.sym) {
            case SDLK_w: if (mJumpsRemaining > 0) {mVelY = -mJumpVelMax; mJumpsRemaining--;} break;
            case SDLK_a: mVelX -= mPlayerVel; break;
            case SDLK_d: mVelX += mPlayerVel; break;
        }
    } else if(e->type == SDL_KEYUP && e->key.repeat == 0) {
        switch(e->key.keysym.sym)
        {
            case SDLK_w: if (mVelY < -mJumpVelMin) mVelY = -mJumpVelMin; break;
            case SDLK_a: mVelX += mPlayerVel; break;
            case SDLK_d: mVelX -= mPlayerVel; break;
        }
    }
}
void LPlayer::move(std::vector<LTile*>& tiles, float timeStep)
{
    SDL_Rect tempBox = mCollisionBox;
    mCollisionBox.x += mVelX * timeStep;
    if(mCollisionBox.x < 0) mCollisionBox.x = 0;
    else if(mCollisionBox.x > levelDimensions[save.level - 1].w - PLAYER_WIDTH) mCollisionBox.x = levelDimensions[save.level - 1].w - PLAYER_WIDTH;
    mVelY += mGravity * timeStep;
    mCollisionBox.y += mVelY * timeStep;
    if (mVelY > 6 * mGravity) mVelY = 6 * mGravity;
    if(mCollisionBox.y < 0) mCollisionBox.y = 0;
    else if(mCollisionBox.y > levelDimensions[save.level - 1].h - PLAYER_HEIGHT) mCollisionBox.y = levelDimensions[save.level - 1].h - PLAYER_HEIGHT;
    if (touchesTile(tiles)) {
        SDL_Point point = getNearestCollision(mVelX, 0, tempBox, tiles);
        mCollisionBox.x = point.x;
        point = getNearestCollision(0, mVelY, tempBox, tiles);
        mCollisionBox.y = point.y;  
    }
    if (touchesGround(tiles)) {
        mVelY = 0;
        mJumpsRemaining = 1;
    }
    if (touchesCeiling(tiles) && mVelY < 0) mVelY = 0;
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
    if(camera.x > levelDimensions[save.level - 1].w - camera.w)
        camera.x = levelDimensions[save.level - 1].w - camera.w;
    if(camera.y > levelDimensions[save.level - 1].h - camera.h)
        camera.y = levelDimensions[save.level - 1].h - camera.h;
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
bool LPlayer::touchesTile(std::vector<LTile*>& tiles)
{
    for(int i = 0; i < tileCount; i++)
    {
        if(tiles[i]->getType() > TILE_EMPTY && checkCollision(mCollisionBox, tiles[i]->getBox()))
        {
            return true;
        }
    }
    return false;
}
bool LPlayer::touchesGround(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.y == levelDimensions[save.level - 1].h - PLAYER_HEIGHT) return true; 
    SDL_Rect groundBox = {mCollisionBox.x, mCollisionBox.y + PLAYER_HEIGHT, mCollisionBox.w, 1};
    for(int i = 0; i < tileCount; i++)
    {
        if(tiles[i]->getType() > TILE_EMPTY && checkCollision(groundBox, tiles[i]->getBox()))
        {
            return true;
        }
    }
    return false;
}
bool LPlayer::touchesCeiling(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.y == 0) return true; 
    SDL_Rect ceilingBox = {mCollisionBox.x, mCollisionBox.y - 1, mCollisionBox.w, 1};
    for(int i = 0; i < tileCount; i++)
    {
        if(tiles[i]->getType() > TILE_EMPTY && checkCollision(ceilingBox, tiles[i]->getBox()))
        {
            return true;
        }
    }
    return false;
}
bool LPlayer::touchesWallRight(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.x == levelDimensions[save.level - 1].w - PLAYER_WIDTH) return true; 
    SDL_Rect rightBox = {mCollisionBox.x + PLAYER_WIDTH, mCollisionBox.y, 1, mCollisionBox.h};
    for(int i = 0; i < tileCount; i++)
    {
        if(tiles[i]->getType() > TILE_EMPTY && checkCollision(rightBox, tiles[i]->getBox()))
        {
            return true;
        }
    }
    return false;
}
bool LPlayer::touchesWallLeft(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.x == 0) return true; 
    SDL_Rect leftBox = {mCollisionBox.x - 1, mCollisionBox.y, 1, mCollisionBox.h};
    for(int i = 0; i < tileCount; i++)
    {
        if(tiles[i]->getType() > TILE_EMPTY && checkCollision(leftBox, tiles[i]->getBox()))
        {
            return true;
        }
    }
    return false;
}
SDL_Point LPlayer::getNearestCollision(int xVel, int yVel, SDL_Rect oldBox, std::vector<LTile*>& tiles)
{
    SDL_Point point = {mCollisionBox.x, mCollisionBox.y};
    for(int i = 0; i < tileCount; i++)
    {
        if(tiles[i]->getType() > TILE_EMPTY && checkCollision(mCollisionBox, tiles[i]->getBox()))
        {
            if(xVel > 0 && oldBox.y > tiles[i]->getBox().y - oldBox.h && oldBox.y < tiles[i]->getBox().y + tiles[i]->getBox().h) {
                point.x = tiles[i]->getBox().x - oldBox.w;
            } else if(xVel < 0 && oldBox.y > tiles[i]->getBox().y - oldBox.h && oldBox.y < tiles[i]->getBox().y + tiles[i]->getBox().h) {
                point.x = tiles[i]->getBox().x + tiles[i]->getBox().w;
            }
            if(yVel > 0 && oldBox.x > tiles[i]->getBox().x - oldBox.w && oldBox.x < tiles[i]->getBox().x + tiles[i]->getBox().w) {
                point.y = tiles[i]->getBox().y - oldBox.h;
            } else if(yVel < 0 && oldBox.x > tiles[i]->getBox().x - oldBox.w && oldBox.x < tiles[i]->getBox().x + tiles[i]->getBox().w) {
                point.y = tiles[i]->getBox().y + tiles[i]->getBox().h;
            }
        }
    }
    return point;
}