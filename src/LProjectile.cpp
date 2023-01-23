#include "LProjectile.h"
#include "Game.h"

SDL_Rect tempclips[1] = {
    { 0,  0, 25, 25}
};

LProjectile::LProjectile(int x, int y, int w, int h, int velX, int velY, int gravity, int type)
{
    mFrame = 0;
    mCollisionBox = {x, y, w, h};
    mVelX = velX;
    mVelY = velY;
    mGravity = gravity;
    mType = type;
    mDestroyOnPlayerCollision = true;
    mDestroyOnTileCollision = false;
    mAnimationSpeed = 10;
}
void LProjectile::move(std::vector<LTile*>& tiles, float timeStep)
{
    if (mVelX == 0 && mVelY == 0) return;
    SDL_Rect tempBox = mCollisionBox;
    mCollisionBox.x += mVelX * timeStep;
    if(mCollisionBox.x < 0) mCollisionBox.x = 0;
    else if(mCollisionBox.x > levelDimensions[save.level - 1].w - mCollisionBox.w) mCollisionBox.x = levelDimensions[save.level - 1].w - mCollisionBox.w;
    if (!touchesGround(tiles)) mVelY += mGravity * timeStep;
    mCollisionBox.y += mVelY * timeStep;
    if (mVelY > 6 * mGravity) mVelY = 6 * mGravity;
    if(mCollisionBox.y < 0) mCollisionBox.y = 0;
    else if(mCollisionBox.y > levelDimensions[save.level - 1].h - mCollisionBox.h) mCollisionBox.y = levelDimensions[save.level - 1].h - mCollisionBox.h;
    if (touchesTile(tiles)) {
        if (mDestroyOnTileCollision) {
            projectileEvent(tiles);
            mVelX = 0;
            mVelY = 0;
            mCollisionBox.x = -mCollisionBox.w;
            mCollisionBox.y = -mCollisionBox.h;
            return;
        }
        SDL_Point point = getNearestCollision(mVelX, 0, tempBox, tiles);
        mCollisionBox.x = point.x;
        point = getNearestCollision(0, mVelY, tempBox, tiles);
        mCollisionBox.y = point.y;
    }
    if (mDestroyOnPlayerCollision && checkCollision(mCollisionBox, player->getBox())) {
        projectileEvent(tiles);
        mVelX = 0;
        mVelY = 0;
        mCollisionBox.x = -mCollisionBox.w;
        mCollisionBox.y = -mCollisionBox.h;
        return;
    }
    if (touchesCeiling(tiles) && mVelY < 0) mVelY = 0;
    if (touchesGround(tiles)) mVelX *= 0.8;
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
    projectileTexture.render((int)mCollisionBox.x - camera.x, (int)mCollisionBox.y - camera.y, &tempclips[mFrame / mAnimationSpeed]);
}
int LProjectile::getPosX()
{
    return (int)mCollisionBox.x;
}
int LProjectile::getPosY()
{
    return (int)mCollisionBox.y;
}
void LProjectile::projectileEvent(std::vector<LTile*>& tiles)
{
    switch (mType) {
        case PROJECTILE_HEART:
            if (player->getHealth() < save.maxHealth) player->setHealth(player->getHealth() + 1);
            break;
        case PROJECTILE_SHIELD:
            break;
        case PROJECTILE_DAMAGEBALL:
            player->setHealth(player->getHealth() - 1);
            break;
        case PROJECTILE_OPENER:
            break;
    }
}
bool LProjectile::touchesTile(std::vector<LTile*>& tiles)
{
    int topLeftTile = ((int)(mCollisionBox.y / LTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH) + (int)(mCollisionBox.x / LTile::TILE_WIDTH) - 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topLeftTile + i * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH);
        if (curTile < 0 || curTile >= tileCount) continue;
        if((tiles[curTile]->getType() > TILE_EMPTY || tiles[curTile]->getType() == TILE_GHOST_D) && checkCollision(mCollisionBox, tiles[curTile]->getBox())) return true;
        if((tiles[curTile + 1]->getType() > TILE_EMPTY || tiles[curTile + 1]->getType() == TILE_GHOST_D) && checkCollision(mCollisionBox, tiles[curTile + 1]->getBox())) return true;
        if((tiles[curTile + 2]->getType() > TILE_EMPTY || tiles[curTile + 2]->getType() == TILE_GHOST_D) && checkCollision(mCollisionBox, tiles[curTile + 2]->getBox())) return true;
    }
    return false;
}
bool LProjectile::touchesGround(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.y == levelDimensions[save.level - 1].h - mCollisionBox.h) return true; 
    SDL_Rect groundBox = {mCollisionBox.x, mCollisionBox.y + mCollisionBox.h, mCollisionBox.w, 1};
    int bottomLeftTile = ((int)(mCollisionBox.y / LTile::TILE_HEIGHT) + 1) * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH) + (int)(mCollisionBox.x / LTile::TILE_WIDTH) - 1;
    for (int i = bottomLeftTile; i < bottomLeftTile + 3; i++) {
        if (i >= tileCount) continue;
        if((tiles[i]->getType() > TILE_EMPTY || tiles[i]->getType() == TILE_GHOST_D) && checkCollision(groundBox, tiles[i]->getBox())) return true;
    }
    return false;
}
bool LProjectile::touchesCeiling(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.y == 0) return true; 
    SDL_Rect ceilingBox = {mCollisionBox.x, mCollisionBox.y - 1, mCollisionBox.w, 1};
    int topLeftTile = ((int)(mCollisionBox.y / LTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH) + (int)(mCollisionBox.x / LTile::TILE_WIDTH) - 1;
    for (int i = topLeftTile; i < topLeftTile + 3; i++) {
        if (i < 0) continue;
        if((tiles[i]->getType() > TILE_EMPTY || tiles[i]->getType() == TILE_GHOST_D) && checkCollision(ceilingBox, tiles[i]->getBox())) return true;
    }
    return false;
}
bool LProjectile::touchesWallRight(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.x == levelDimensions[save.level - 1].w - mCollisionBox.w) return true; 
    SDL_Rect rightBox = {mCollisionBox.x + mCollisionBox.w, mCollisionBox.y, 1, mCollisionBox.h};
    int topRightTile = ((int)(mCollisionBox.y / LTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH) + (int)(mCollisionBox.x / LTile::TILE_WIDTH) + 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topRightTile + i * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH);
        if (curTile < 0 || curTile >= tileCount) continue;
        if((tiles[curTile]->getType() > TILE_EMPTY || tiles[curTile]->getType() == TILE_GHOST_D) && checkCollision(rightBox, tiles[curTile]->getBox())) return true;
    }
    return false;
}
bool LProjectile::touchesWallLeft(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.x == 0) return true; 
    SDL_Rect leftBox = {mCollisionBox.x - 1, mCollisionBox.y, 1, mCollisionBox.h};
    int topLeftTile = ((int)(mCollisionBox.y / LTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH) + (int)(mCollisionBox.x / LTile::TILE_WIDTH) - 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topLeftTile + i * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH);
        if (curTile < 0 || curTile >= tileCount) continue;
        if((tiles[curTile]->getType() > TILE_EMPTY || tiles[curTile]->getType() == TILE_GHOST_D) && checkCollision(leftBox, tiles[curTile]->getBox())) return true;
    }
    return false;
}
SDL_Point LProjectile::getNearestCollision(int xVel, int yVel, SDL_Rect oldBox, std::vector<LTile*>& tiles)
{
    SDL_Point point = {mCollisionBox.x, mCollisionBox.y};
    for (int i = 0; i < tileCount; i++) {
        if((tiles[i]->getType() > TILE_EMPTY || tiles[i]->getType() == TILE_GHOST_D) && checkCollision(mCollisionBox, tiles[i]->getBox())) {
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