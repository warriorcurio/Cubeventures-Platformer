#include "LProjectile.h"
#include "Game.h"

SDL_Rect tempclips[1] = {
    { 0,  0, 25, 25}
};

LProjectile::LProjectile(int x, int y, int w, int h, int velX, int velY)
{
    mTexture.loadFromFile("res/heartItem.png");
    mFrame = 0;
    mCollisionBox = {x, y, w, h};
    mW = w;
    mH = h;
    mBounceFactor = 0.5;
    mVelX = velX;
    mVelY = velY;
    mGravity = 1000;
    mAnimationSpeed = 10;
}
LProjectile::~LProjectile()
{
    mTexture.free();
}
void LProjectile::move(std::vector<LTile*>& tiles, float timeStep)
{
    SDL_Rect tempBox = mCollisionBox;
    mCollisionBox.x += mVelX * timeStep;
    if(mCollisionBox.x < 0) mCollisionBox.x = 0;
    else if(mCollisionBox.x > levelDimensions[save.level - 1].w - mW) mCollisionBox.x = levelDimensions[save.level - 1].w - mW;
    mVelY += mGravity * timeStep;
    mCollisionBox.y += mVelY * timeStep;
    if (mVelY > 6 * mGravity) mVelY = 6 * mGravity;
    if(mCollisionBox.y < 0) mCollisionBox.y = 0;
    else if(mCollisionBox.y > levelDimensions[save.level - 1].h - mH) mCollisionBox.y = levelDimensions[save.level - 1].h - mH;
    if (touchesTile(tiles)) {
        SDL_Point point = getNearestCollision(mVelX, 0, tempBox, tiles);
        mCollisionBox.x = point.x;
        point = getNearestCollision(0, mVelY, tempBox, tiles);
        mCollisionBox.y = point.y;  
    }
    
    if (touchesGround(tiles) || (touchesCeiling(tiles) && mVelY < 0)) {
        if (touchesGround(tiles) && mVelY < 150 && mVelY >= 0) {
            mVelY = 0;
            mVelX *= 0.8;
        }
        mVelY = -mVelY * mBounceFactor;
    }
    if (touchesWallLeft(tiles) || touchesWallRight(tiles)) mVelX = -mVelX * mBounceFactor;
}
void LProjectile::setPos(int x, int y)
{
    mCollisionBox.x = x;
    mCollisionBox.y = y;
    save.x = mCollisionBox.x;
    save.y = mCollisionBox.y;
}
void LProjectile::render(SDL_Rect& camera)
{
    mFrame = (mFrame + 1) % mAnimationSpeed;
    mTexture.render((int)mCollisionBox.x - camera.x, (int)mCollisionBox.y - camera.y, &tempclips[mFrame / 10]);
}
int LProjectile::getPosX()
{
    return (int)mCollisionBox.x;
}
int LProjectile::getPosY()
{
    return (int)mCollisionBox.y;
}
bool LProjectile::touchesTile(std::vector<LTile*>& tiles)
{
    for(int i = 0; i < tileCount; i++)
    {
        if((tiles[i]->getType() > TILE_EMPTY || tiles[i]->getType() == TILE_GHOST_D) && checkCollision(mCollisionBox, tiles[i]->getBox()))
        {
            return true;
        }
    }
    return false;
}
bool LProjectile::touchesGround(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.y == levelDimensions[save.level - 1].h - mH) return true; 
    SDL_Rect groundBox = {mCollisionBox.x, mCollisionBox.y + mH, mCollisionBox.w, 1};
    for(int i = 0; i < tileCount; i++)
    {
        if((tiles[i]->getType() > TILE_EMPTY || tiles[i]->getType() == TILE_GHOST_D) && checkCollision(groundBox, tiles[i]->getBox()))
        {
            return true;
        }
    }
    return false;
}
bool LProjectile::touchesCeiling(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.y == 0) return true; 
    SDL_Rect ceilingBox = {mCollisionBox.x, mCollisionBox.y - 1, mCollisionBox.w, 1};
    for(int i = 0; i < tileCount; i++)
    {
        if((tiles[i]->getType() > TILE_EMPTY || tiles[i]->getType() == TILE_GHOST_D) && checkCollision(ceilingBox, tiles[i]->getBox()))
        {
            return true;
        }
    }
    return false;
}
bool LProjectile::touchesWallRight(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.x == levelDimensions[save.level - 1].w - mW) return true; 
    SDL_Rect rightBox = {mCollisionBox.x + mW, mCollisionBox.y, 1, mCollisionBox.h};
    for(int i = 0; i < tileCount; i++)
    {
        if((tiles[i]->getType() > TILE_EMPTY || tiles[i]->getType() == TILE_GHOST_D) && checkCollision(rightBox, tiles[i]->getBox()))
        {
            return true;
        }
    }
    return false;
}
bool LProjectile::touchesWallLeft(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.x == 0) return true; 
    SDL_Rect leftBox = {mCollisionBox.x - 1, mCollisionBox.y, 1, mCollisionBox.h};
    for(int i = 0; i < tileCount; i++)
    {
        if((tiles[i]->getType() > TILE_EMPTY || tiles[i]->getType() == TILE_GHOST_D) && checkCollision(leftBox, tiles[i]->getBox()))
        {
            return true;
        }
    }
    return false;
}
SDL_Point LProjectile::getNearestCollision(int xVel, int yVel, SDL_Rect oldBox, std::vector<LTile*>& tiles)
{
    SDL_Point point = {mCollisionBox.x, mCollisionBox.y};
    for(int i = 0; i < tileCount; i++)
    {
        if((tiles[i]->getType() > TILE_EMPTY || tiles[i]->getType() == TILE_GHOST_D) && checkCollision(mCollisionBox, tiles[i]->getBox()))
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