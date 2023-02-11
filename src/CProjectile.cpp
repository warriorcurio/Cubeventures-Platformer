#include "CProjectile.h"
#include "Game.h"

int numFrames[PROJECTILE_TOTAL] = {1, 1, 4, 4, 1, 1, 4, 1, 1, 1};

CProjectile::CProjectile(int x, int y, ProjectileTypes type, int velX, int velY, int respawnX, int respawnY)
{
    mFrame = 0;
    mCollisionBox = {x, y, 25, 25};
    mVelX = velX;
    mVelY = velY;
    if (respawnX == -1) mUtilityX = x;
    else mUtilityX = respawnX;
    if (respawnY == -1) mUtilityY = y;
    else mUtilityY = respawnY;
    mHasActivated = false;
    if (type != PROJECTILE_CHARGER && type != PROJECTILE_DAMAGEBALL) mGravity = 1000;
    else mGravity = 0;
    mType = type;
    mActivateOnPlayerCollision = true;
    mDestroyOnPlayerCollision = true;
    mActivateOnPlayerLeave = false;
    mActivateOnTileCollision = mType == PROJECTILE_DAMAGEBALL;
    mDestroyOnTileCollision = false;
    mAnimationSpeed = 10;
}
CProjectile::CProjectile(int x, int y, int editTileIndex, Tiles editTileNew, bool isOneTimeOnly)
{
    mFrame = 0;
    mCollisionBox = {x, y, 25, 6};
    mVelX = 0;
    mVelY = 0;
    mEditTileIndex = editTileIndex;
    mEditTileOriginal = tiles[editTileIndex]->getType();
    mEditTileNew = editTileNew;
    mHasActivated = false;
    mGravity = 0;
    mType = PROJECTILE_BUTTON_TILECHANGE;
    mActivateOnPlayerCollision = true;
    mDestroyOnPlayerCollision = isOneTimeOnly;
    mActivateOnPlayerLeave = false;
    mActivateOnTileCollision = false;
    mDestroyOnTileCollision = false;
    mAnimationSpeed = 1;
}
CProjectile::CProjectile(int x, int y, int editTileIndex, Tiles editTileOriginal, Tiles editTileNew)
{
    mFrame = 0;
    mCollisionBox = {x, y, 25, 6};
    mVelX = 0;
    mVelY = 0;
    mEditTileIndex = editTileIndex;
    mEditTileOriginal = editTileOriginal;
    mEditTileNew = editTileNew;
    mHasActivated = false;
    mGravity = 0;
    mType = PROJECTILE_BUTTON_TILETOGGLE;
    mActivateOnPlayerCollision = true;
    mDestroyOnPlayerCollision = false;
    mActivateOnPlayerLeave = false;
    mActivateOnTileCollision = false;
    mDestroyOnTileCollision = false;
    mAnimationSpeed = 1;
}
CProjectile::CProjectile(int x, int y, int teleportX, int teleportY)
{
    mFrame = 0;
    mCollisionBox = {x, y, 25, 25};
    mVelX = 0;
    mVelY = 0;
    mUtilityX = teleportX;
    mUtilityY = teleportY;
    mGravity = 0;
    mType = PROJECTILE_TELEPORTER;
    mActivateOnPlayerCollision = true;
    mDestroyOnPlayerCollision = false;
    mActivateOnPlayerLeave = false;
    mActivateOnTileCollision = false;
    mDestroyOnTileCollision = false;
    mAnimationSpeed = 8;
}
CProjectile::CProjectile(int x, int y, int w, int h, ProjectileTypes type)
{
    mFrame = 0;
    mCollisionBox = {x, y, w, h};
    mVelX = 0;
    mVelY = 0;
    mHasActivated = false;
    mGravity = 0;
    mType = type;
    mActivateOnPlayerCollision = true;
    mDestroyOnPlayerCollision = false;
    mActivateOnPlayerLeave = true;
    mActivateOnTileCollision = false;
    mDestroyOnTileCollision = false;
    mAnimationSpeed = 1;
}
CProjectile::CProjectile(int x, int y, int w, int h, int textX, int textY, const char* textToDisplay, SDL_Color textColour, int size)
{
    mFrame = 0;
    mCollisionBox = {x, y, w, h};
    mVelX = 0;
    mVelY = 0;
    mUtilityX = textX;
    mUtilityY = textY;
    mHasActivated = false;
    mGravity = 0;
    mType = PROJECTILE_TEXTDISPLAYER;
    std::strcpy(mTextToDisplay, textToDisplay);
    mTextColour = textColour;
    mTextSize = size;
    mTextTexture.loadFromRenderedText(mTextToDisplay, mTextColour, mTextSize);
    mDisplayText = false;
    mActivateOnPlayerCollision = true;
    mDestroyOnPlayerCollision = false;
    mActivateOnPlayerLeave = false;
    mActivateOnTileCollision = false;
    mDestroyOnTileCollision = false;
    mAnimationSpeed = 1;
}
CProjectile::~CProjectile()
{
    mTextTexture.free();
}
void CProjectile::move(float timeStep)
{
    if (mActivateOnPlayerCollision && checkCollision(mCollisionBox, player->getBox()) && !mHasActivated) {
        projectileEvent();
        mHasActivated = true;
    } else if (!checkCollision(mCollisionBox, player->getBox())) {
        if (mHasActivated && mActivateOnPlayerLeave) projectileEvent();
        mHasActivated = false;
    }
    if (mDestroyOnPlayerCollision && checkCollision(mCollisionBox, player->getBox())) destroySelf();
    if (mVelX == 0 && mVelY == 0 && (mGravity == 0 || (mCollisionBox.x == -mCollisionBox.w && mCollisionBox.y == -mCollisionBox.h))) return;
    SDL_Rect tempBox = mCollisionBox;
    mCollisionBox.x += mVelX * timeStep;
    if(mCollisionBox.x < 0) {
        mCollisionBox.x = 0;
        if (mActivateOnTileCollision) projectileEvent();
        if (mDestroyOnTileCollision) destroySelf();
    }
    else if(mCollisionBox.x > levelDimensions[save.level].w - mCollisionBox.w) {
        mCollisionBox.x = levelDimensions[save.level].w - mCollisionBox.w;
        if (mActivateOnTileCollision) projectileEvent();
        if (mDestroyOnTileCollision) destroySelf();
    }
    if (!touchesGround()) mVelY += mGravity * timeStep;
    mCollisionBox.y += mVelY * timeStep;
    if (mVelY > 6 * mGravity) mVelY = 6 * mGravity;
    if(mCollisionBox.y < 0) {
        mCollisionBox.y = 0;
        if (mActivateOnTileCollision) projectileEvent();
        if (mDestroyOnTileCollision) destroySelf();
    }
    else if(mCollisionBox.y > levelDimensions[save.level].h - mCollisionBox.h) {
        mCollisionBox.y = levelDimensions[save.level].h - mCollisionBox.h;
        if (mActivateOnTileCollision) projectileEvent();
        if (mDestroyOnTileCollision) destroySelf();
    }
    if (touchesTile()) {
        if (mActivateOnTileCollision) projectileEvent();
        if (mDestroyOnTileCollision) destroySelf();
        SDL_Point point = getNearestCollision(mVelX, 0, tempBox);
        mCollisionBox.x = point.x;
        point = getNearestCollision(0, mVelY, tempBox);
        mCollisionBox.y = point.y;
    }
    if (mActivateOnPlayerCollision && checkCollision(mCollisionBox, player->getBox()) && !mHasActivated) {
        projectileEvent();
        mHasActivated = true;
    } else if (!(mActivateOnPlayerCollision && checkCollision(mCollisionBox, player->getBox()))) mHasActivated = false;
    if (mDestroyOnPlayerCollision && checkCollision(mCollisionBox, player->getBox())) destroySelf();
    if (touchesCeiling() && mVelY < 0) mVelY = 0;
    if (touchesGround()) mVelX *= 0.8;
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
    projectileTexture.render(mCollisionBox.x - camera.x, mCollisionBox.y - camera.y + mCollisionBox.h - 25, &renderRect);
    if (mDisplayText) mTextTexture.render(mUtilityX - camera.x, mUtilityY- camera.y);
}
int CProjectile::getPosX()
{
    return (int)mCollisionBox.x;
}
int CProjectile::getPosY()
{
    return (int)mCollisionBox.y;
}
int CProjectile::getType()
{
    return mType;
}
int CProjectile::getEditTileIndex()
{
    if (mType == PROJECTILE_BUTTON_TILECHANGE) return mEditTileIndex;
    else return -1;
}
int CProjectile::getEditTileOriginal()
{
    if (mType == PROJECTILE_BUTTON_TILECHANGE) return mEditTileOriginal;
    else return -1;
}
void CProjectile::projectileEvent()
{
    switch (mType) {
        case PROJECTILE_HEART:
            if (player->getHealth() < save.maxHealth) player->setHealth(player->getHealth() + 1);
            break;
        case PROJECTILE_SHIELD:
            player->setShield(player->getShield() + 1);
            break;
        case PROJECTILE_CHARGER:
            if (player->getCharge() < CPlayer::MAX_CHARGE && player->getForm() != FORM_RAINBOW) player->setCharge(player->getCharge() + 1);
            break;
        case PROJECTILE_DAMAGEBALL:
            if (!checkCollision(mCollisionBox, player->getBox())) {
                mCollisionBox.x = mUtilityX;
                mCollisionBox.y = mUtilityY;
                break;
            }
            if (player->getInvulnerable() || player->getForm() == FORM_RAINBOW) break;
            if (player->getShield() > 0) player->setShield(player->getShield() - 1);
            else player->setHealth(player->getHealth() - 1);
            player->setPos(player->getSafePos().x, player->getSafePos().y);
            if (player->getHealth() == 0) {
                save.deaths++;
                player->setHealth(save.maxHealth);
                player->setKeys(0);
                isDead = true;
                setLevel(save.level);
            } else player->setInvulnerable(true);
            destroySelf();
            break;
        case PROJECTILE_BUTTON_TILECHANGE:
            tiles[mEditTileIndex]->setType(mEditTileNew);
            mEditTileNew = mEditTileOriginal;
            mEditTileOriginal = tiles[mEditTileIndex]->getType();
            break;
        case PROJECTILE_BUTTON_TILETOGGLE:
            if (tiles[mEditTileIndex]->getType() == mEditTileOriginal) tiles[mEditTileIndex]->setType(mEditTileNew);
            else tiles[mEditTileIndex]->setType(mEditTileOriginal);
            break;
        case PROJECTILE_TELEPORTER:
            player->setPos(mUtilityX, mUtilityY);
            save.x = player->getSafePos().x;
            save.y = player->getSafePos().y;
            break;
        case PROJECTILE_SAVER:
            player->setSafePos(player->getBox().x, player->getBox().y);
            break;
        case PROJECTILE_TEXTDISPLAYER:
            mDisplayText = true;
            break;
        case PROJECTILE_BOUNCEBLOCK:
            if (checkCollision(mCollisionBox, player->getBox())) player->setCustomForm(player->getPlayerVel() * 0.5, player->getJumpMax() * 1.414, player->getJumpMin() * 1.414, player->getGravity());
            else player->setForm(player->getForm());
            break;
    }
}
void CProjectile::destroySelf()
{
    mVelX = 0;
    mVelY = 0;
    mCollisionBox.x = -mCollisionBox.w;
    mCollisionBox.y = -mCollisionBox.h;
    return;
}
bool CProjectile::touchesTile()
{
    int topLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topLeftTile + i * (levelDimensions[save.level].w / CTile::TILE_WIDTH);
        if(curTile >= 0 && curTile < tileCount && (tiles[curTile]->getType() > TILE_EMPTY || tiles[curTile]->getType() == TILE_GHOST_T_D || tiles[curTile]->getType() == TILE_GHOST_F_D) && checkCollision(mCollisionBox, tiles[curTile]->getBox())) return true;
        if(curTile + 1 >= 0 && curTile + 1 < tileCount && (tiles[curTile + 1]->getType() > TILE_EMPTY || tiles[curTile + 1]->getType() == TILE_GHOST_T_D || tiles[curTile + 2]->getType() == TILE_GHOST_F_D) && checkCollision(mCollisionBox, tiles[curTile + 1]->getBox())) return true;
        if(curTile + 2 >= 0 && curTile + 2 < tileCount && (tiles[curTile + 2]->getType() > TILE_EMPTY || tiles[curTile + 2]->getType() == TILE_GHOST_T_D || tiles[curTile + 2]->getType() == TILE_GHOST_F_D) && checkCollision(mCollisionBox, tiles[curTile + 2]->getBox())) return true;
    }
    return false;
}
bool CProjectile::touchesGround()
{
    if (mCollisionBox.y == levelDimensions[save.level].h - mCollisionBox.h) return true; 
    SDL_Rect groundBox = {mCollisionBox.x, mCollisionBox.y + mCollisionBox.h, mCollisionBox.w, 1};
    int bottomLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) + 1) * (levelDimensions[save.level].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = bottomLeftTile; i < bottomLeftTile + 3; i++) {
        if(i < tileCount && (tiles[i]->getType() > TILE_EMPTY || tiles[i]->getType() == TILE_GHOST_T_D || tiles[i]->getType() == TILE_GHOST_F_D) && checkCollision(groundBox, tiles[i]->getBox())) return true;
    }
    return false;
}
bool CProjectile::touchesCeiling()
{
    if (mCollisionBox.y == 0) return true; 
    SDL_Rect ceilingBox = {mCollisionBox.x, mCollisionBox.y - 1, mCollisionBox.w, 1};
    int topLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = topLeftTile; i < topLeftTile + 3; i++) {
        if(i >= 0 && (tiles[i]->getType() > TILE_EMPTY || tiles[i]->getType() == TILE_GHOST_T_D || tiles[i]->getType() == TILE_GHOST_F_D) && checkCollision(ceilingBox, tiles[i]->getBox())) return true;
    }
    return false;
}
SDL_Point CProjectile::getNearestCollision(int xVel, int yVel, SDL_Rect oldBox)
{
    SDL_Point point = {mCollisionBox.x, mCollisionBox.y};
    int topLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topLeftTile + i * (levelDimensions[save.level].w / CTile::TILE_WIDTH);
        for (int j = curTile; j < curTile + 3; j++) {
            if (j < 0 || j > tileCount) continue;
            if ((tiles[j]->getType() > TILE_EMPTY || tiles[j]->getType() == TILE_GHOST_T_D || tiles[j]->getType() == TILE_GHOST_F_D) && checkCollision(mCollisionBox, tiles[j]->getBox())) {
                if(xVel > 0 && oldBox.y > tiles[j]->getBox().y - oldBox.h && oldBox.y < tiles[j]->getBox().y + tiles[j]->getBox().h) point.x = tiles[j]->getBox().x - oldBox.w;
                else if(xVel < 0 && oldBox.y > tiles[j]->getBox().y - oldBox.h && oldBox.y < tiles[j]->getBox().y + tiles[j]->getBox().h) point.x = tiles[j]->getBox().x + tiles[j]->getBox().w;
                if(yVel > 0 && oldBox.x > tiles[j]->getBox().x - oldBox.w && oldBox.x < tiles[j]->getBox().x + tiles[j]->getBox().w) point.y = tiles[j]->getBox().y - oldBox.h;
                else if(yVel < 0 && oldBox.x > tiles[j]->getBox().x - oldBox.w && oldBox.x < tiles[j]->getBox().x + tiles[j]->getBox().w) point.y = tiles[j]->getBox().y + tiles[j]->getBox().h;
            }
        }
    }
    return point;
}