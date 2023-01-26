#include "CProjectile.h"
#include "Game.h"

int numFrames[PROJECTILE_TOTAL] = {1, 1, 4, 1, 1};

CProjectile::CProjectile(int x, int y, int w, int h, int velX, int velY, int gravity, int type)
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
void CProjectile::move(std::vector<CTile*>& tiles, float timeStep)
{
    if (mDestroyOnPlayerCollision && checkCollision(mCollisionBox, player->getBox())) {
        projectileEvent(tiles);
        mVelX = 0;
        mVelY = 0;
        mCollisionBox.x = -mCollisionBox.w;
        mCollisionBox.y = -mCollisionBox.h;
        return;
    }
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
void CProjectile::setPos(int x, int y)
{
    mCollisionBox.x = x;
    mCollisionBox.y = y;
}
void CProjectile::render(SDL_Rect& camera)
{
    mFrame = (mFrame + 1) % (numFrames[mType] * mAnimationSpeed);
    SDL_Rect renderRect = {mType * 25, (int)(mFrame / mAnimationSpeed) * 25, 25, 25};
    projectileTexture.render((int)mCollisionBox.x - camera.x, (int)mCollisionBox.y - camera.y, &renderRect);
}
int CProjectile::getPosX()
{
    return (int)mCollisionBox.x;
}
int CProjectile::getPosY()
{
    return (int)mCollisionBox.y;
}
void CProjectile::projectileEvent(std::vector<CTile*>& tiles)
{
    switch (mType) {
        case PROJECTILE_HEART:
            if (player->getHealth() < save.maxHealth) player->setHealth(player->getHealth() + 1);
            break;
        case PROJECTILE_SHIELD:
            player->setShield(player->getShield() + 1);
            break;
        case PROJECTILE_DAMAGEBALL:
            if (player->getInvulnerable() || player->getForm() == FORM_RAINBOW) return;
            if (player->getShield() > 0) player->setShield(player->getShield() - 1);
            else player->setHealth(player->getHealth() - 1);
            player->setPos(player->getSafePos().x, player->getSafePos().y);
            if (player->getHealth() == 0) {
                save.deaths++;
                player->setHealth(save.maxHealth);
                player->setKeys(0);
                setLevel(save.level);
                isDead = true;
            } else player->setInvulnerable(true);
            break;
        case PROJECTILE_BUTTON:
            break;
        case PROJECTILE_CHARGER:
            if (player->getCharge() < 100) player->setCharge(player->getCharge() + 1);
            break;
    }
}
bool CProjectile::touchesTile(std::vector<CTile*>& tiles)
{
    int topLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topLeftTile + i * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH);
        if (curTile < 0 || curTile >= tileCount) continue;
        if((tiles[curTile]->getType() > TILE_EMPTY || tiles[curTile]->getType() == TILE_GHOST_T_D || tiles[curTile]->getType() == TILE_GHOST_F_D) && checkCollision(mCollisionBox, tiles[curTile]->getBox())) return true;
        if((tiles[curTile + 1]->getType() > TILE_EMPTY || tiles[curTile + 1]->getType() == TILE_GHOST_T_D) && checkCollision(mCollisionBox, tiles[curTile + 1]->getBox())) return true;
        if((tiles[curTile + 2]->getType() > TILE_EMPTY || tiles[curTile + 2]->getType() == TILE_GHOST_T_D) && checkCollision(mCollisionBox, tiles[curTile + 2]->getBox())) return true;
    }
    return false;
}
bool CProjectile::touchesGround(std::vector<CTile*>& tiles)
{
    if (mCollisionBox.y == levelDimensions[save.level - 1].h - mCollisionBox.h) return true; 
    SDL_Rect groundBox = {mCollisionBox.x, mCollisionBox.y + mCollisionBox.h, mCollisionBox.w, 1};
    int bottomLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) + 1) * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = bottomLeftTile; i < bottomLeftTile + 3; i++) {
        if (i >= tileCount) continue;
        if((tiles[i]->getType() > TILE_EMPTY || tiles[i]->getType() == TILE_GHOST_T_D) && checkCollision(groundBox, tiles[i]->getBox())) return true;
    }
    return false;
}
bool CProjectile::touchesCeiling(std::vector<CTile*>& tiles)
{
    if (mCollisionBox.y == 0) return true; 
    SDL_Rect ceilingBox = {mCollisionBox.x, mCollisionBox.y - 1, mCollisionBox.w, 1};
    int topLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = topLeftTile; i < topLeftTile + 3; i++) {
        if (i < 0) continue;
        if((tiles[i]->getType() > TILE_EMPTY || tiles[i]->getType() == TILE_GHOST_T_D) && checkCollision(ceilingBox, tiles[i]->getBox())) return true;
    }
    return false;
}
bool CProjectile::touchesWallRight(std::vector<CTile*>& tiles)
{
    if (mCollisionBox.x == levelDimensions[save.level - 1].w - mCollisionBox.w) return true; 
    SDL_Rect rightBox = {mCollisionBox.x + mCollisionBox.w, mCollisionBox.y, 1, mCollisionBox.h};
    int topRightTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) + 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topRightTile + i * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH);
        if (curTile < 0 || curTile >= tileCount) continue;
        if((tiles[curTile]->getType() > TILE_EMPTY || tiles[curTile]->getType() == TILE_GHOST_T_D || tiles[curTile]->getType() == TILE_GHOST_F_D) && checkCollision(rightBox, tiles[curTile]->getBox())) return true;
    }
    return false;
}
bool CProjectile::touchesWallLeft(std::vector<CTile*>& tiles)
{
    if (mCollisionBox.x == 0) return true; 
    SDL_Rect leftBox = {mCollisionBox.x - 1, mCollisionBox.y, 1, mCollisionBox.h};
    int topLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topLeftTile + i * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH);
        if (curTile < 0 || curTile >= tileCount) continue;
        if((tiles[curTile]->getType() > TILE_EMPTY || tiles[curTile]->getType() == TILE_GHOST_T_D || tiles[curTile]->getType() == TILE_GHOST_F_D) && checkCollision(leftBox, tiles[curTile]->getBox())) return true;
    }
    return false;
}
SDL_Point CProjectile::getNearestCollision(int xVel, int yVel, SDL_Rect oldBox, std::vector<CTile*>& tiles)
{
    SDL_Point point = {mCollisionBox.x, mCollisionBox.y};
    int topLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topLeftTile + i * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH);
        for (int j = curTile; j < curTile + 3; j++) {
            if (j < 0 || j > tileCount) continue;
            if ((tiles[j]->getType() > TILE_EMPTY || tiles[j]->getType() == TILE_GHOST_T_D) && checkCollision(mCollisionBox, tiles[j]->getBox())) {
                if(xVel > 0 && oldBox.y > tiles[j]->getBox().y - oldBox.h && oldBox.y < tiles[j]->getBox().y + tiles[j]->getBox().h) point.x = tiles[j]->getBox().x - oldBox.w;
                else if(xVel < 0 && oldBox.y > tiles[j]->getBox().y - oldBox.h && oldBox.y < tiles[j]->getBox().y + tiles[j]->getBox().h) point.x = tiles[j]->getBox().x + tiles[j]->getBox().w;
                if(yVel > 0 && oldBox.x > tiles[j]->getBox().x - oldBox.w && oldBox.x < tiles[j]->getBox().x + tiles[j]->getBox().w) point.y = tiles[j]->getBox().y - oldBox.h;
                else if(yVel < 0 && oldBox.x > tiles[j]->getBox().x - oldBox.w && oldBox.x < tiles[j]->getBox().x + tiles[j]->getBox().w) point.y = tiles[j]->getBox().y + tiles[j]->getBox().h;
            }
        }
    }
    return point;
}