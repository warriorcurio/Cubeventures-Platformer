#include "CProjectile.h"
#include "Game.h"

int numFrames[PROJECTILE_TOTAL] = {1, 1, 4, 4, 1, 1, 4, 1, 1};

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
    if (type != PROJECTILE_CHARGER && type != PROJECTILE_ENERGYBULLET) mGravity = 1000;
    else mGravity = 0;
    mType = type;
    mActivateOnPlayerCollision = true;
    mDestroyOnPlayerCollision = true;
    mActivateOnPlayerLeave = false;
    mActivateOnTileCollision = mType == PROJECTILE_ENERGYBULLET;
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
CProjectile::CProjectile(int x, int y, int w)
{
    mFrame = 0;
    mCollisionBox = {x, y, w, 1};
    mVelX = 0;
    mVelY = 0;
    mHasActivated = false;
    mGravity = 0;
    mType = PROJECTILE_BOUNCE;
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
    //checks to see if the position of the projectile is off the map on the top left
    //if it is, then the projectile is considered destroyed
    //destroyed projectiles do not move at all
    if (mVelX == 0 && mVelY == 0 && (mGravity == 0 || (mCollisionBox.x == -mCollisionBox.w && mCollisionBox.y == -mCollisionBox.h))) return;
    SDL_Rect tempBox = mCollisionBox;
    mCollisionBox.x += (int)(mVelX * timeStep);
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
    mCollisionBox.y += (int)(mVelY * timeStep);
    if (mVelY > mGravity && mGravity != 0) mVelY = mGravity;
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
    if (touchesCeiling() && mVelY < 0 && mGravity != 0) mVelY = 0;
    if (touchesGround()) mVelX *= 0.8;
}
void CProjectile::setPos(int x, int y)
{
    mCollisionBox.x = x;
    mCollisionBox.y = y;
}
void CProjectile::render(SDL_Rect& camera)
{
    if (mDisplayText) mTextTexture.render(mUtilityX - camera.x, mUtilityY- camera.y);
    if (mType > PROJECTILE_TELEPORTER) return;
    //the next frame of animation is after mAnimationSpeed ticks
    mFrame = (mFrame + 1) % (numFrames[mType] * mAnimationSpeed);
    //projectiles are by default 25x25 pixels
    //renderRect is the clip of the projectile type
    SDL_Rect renderRect = {mType * 25, (int)(mFrame / mAnimationSpeed) * 25, 25, 25};
    projectileTexture.render(mCollisionBox.x - camera.x, mCollisionBox.y - camera.y + mCollisionBox.h - 25, &renderRect);
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
        case PROJECTILE_HEART: {
            if (player->getHealth() < save.maxHealth) {
                player->setHealth(player->getHealth() + 1);
                char* temp = (char*)malloc(12);
                strncpy(temp, activity.GetDetails(), 12);
                std::string tempString = (std::string)temp;
                for (int i = 0; i < save.maxHealth + player->getShield(); i++) {
                    if (i < save.curHealth) tempString += "❤️";
                    else if (i < save.maxHealth) tempString += "🖤";
                    else tempString += "💙";
                }
                activity.SetDetails(tempString.c_str());
                core->ActivityManager().UpdateActivity(activity, NULL);
            }
            break;
        }
        case PROJECTILE_SHIELD: {
            player->setShield(player->getShield() + 1);
            std::string tempString = activity.GetDetails();
            tempString += "💙";
            activity.SetDetails(tempString.c_str());
            core->ActivityManager().UpdateActivity(activity, NULL);
            break;
        }
        case PROJECTILE_CHARGER: {
            if (player->getCharge() < CPlayer::MAX_CHARGE && player->getForm() != FORM_RAINBOW) player->setCharge(player->getCharge() + 1);
            break;
        }
        case PROJECTILE_ENERGYBULLET: {
            if (!checkCollision(mCollisionBox, player->getBox())) {
                mCollisionBox.x = mUtilityX;
                mCollisionBox.y = mUtilityY;
                break;
            }
            if (player->getInvulnerable() || player->getForm() == FORM_RAINBOW) break;
            //uses up shield health before regular health
            if (player->getShield() > 0) player->setShield(player->getShield() - 1);
            else player->setHealth(player->getHealth() - 1);
            player->setPos(player->getSafePos().x, player->getSafePos().y);
            if (player->getHealth() == 0) {
                save.deaths++;
                player->setHealth(save.maxHealth);
                player->setKeys(0);
                isDead = true;
                setLevel(save.level);
                Mix_PlayChannel(SFX_DEATH, sfx[SFX_DEATH], 0);
            } else {
                player->setInvulnerable(true);
                Mix_PlayChannel(SFX_HIT, sfx[SFX_HIT], 0);
                char* temp = (char*)malloc(12);
                strncpy(temp, activity.GetDetails(), 12);
                std::string tempString = (std::string)temp;
                for (int i = 0; i < save.maxHealth + player->getShield(); i++) {
                    if (i < save.curHealth) tempString += "❤️";
                    else if (i < save.maxHealth) tempString += "🖤";
                    else tempString += "💙";
                }
                activity.SetDetails(tempString.c_str());
                core->ActivityManager().UpdateActivity(activity, NULL);
            }
            destroySelf();
            break;
        }
        case PROJECTILE_BUTTON_TILECHANGE: {
            tiles[mEditTileIndex]->setType(mEditTileNew);
            switch (mEditTileNew) {
                case TILE_WHITECRYSTAL: Mix_PlayChannel(SFX_CRYSTALREACTIVATE, sfx[SFX_CRYSTALREACTIVATE], 0); break;
                case TILE_REDCRYSTAL: Mix_PlayChannel(SFX_CRYSTALREACTIVATE, sfx[SFX_CRYSTALREACTIVATE], 0); break;
                case TILE_GREENCRYSTAL: Mix_PlayChannel(SFX_CRYSTALREACTIVATE, sfx[SFX_CRYSTALREACTIVATE], 0); break;
                case TILE_BLUECRYSTAL: Mix_PlayChannel(SFX_CRYSTALREACTIVATE, sfx[SFX_CRYSTALREACTIVATE], 0); break;
                case TILE_JUMPCRYSTAL: Mix_PlayChannel(SFX_CRYSTALREACTIVATE, sfx[SFX_CRYSTALREACTIVATE], 0); break;
                default: Mix_PlayChannel(SFX_BREAK, sfx[SFX_BREAK], 0); break;
            }
            mEditTileNew = mEditTileOriginal;
            mEditTileOriginal = tiles[mEditTileIndex]->getType();
            break;
        }
        case PROJECTILE_BUTTON_TILETOGGLE: {
            if (tiles[mEditTileIndex]->getType() == mEditTileOriginal) tiles[mEditTileIndex]->setType(mEditTileNew);
            else tiles[mEditTileIndex]->setType(mEditTileOriginal);
            Mix_PlayChannel(SFX_BREAK, sfx[SFX_BREAK], 0);
            break;
        }
        case PROJECTILE_TELEPORTER: {
            player->setPos(mUtilityX, mUtilityY);
            save.x = player->getSafePos().x;
            save.y = player->getSafePos().y;
            Mix_PlayChannel(SFX_TELEPORT, sfx[SFX_TELEPORT], 0);
            break;
        }
        case PROJECTILE_TEXTDISPLAYER: {
            mDisplayText = true;
            break;
        }
        case PROJECTILE_BOUNCE: {
            if (checkCollision(mCollisionBox, player->getBox())) player->setCustomForm(player->getPlayerVel() * 0.5, player->getJumpMax() * 1.414, player->getJumpMin() * 1.414, player->getGravity());
            else player->setForm(player->getForm());
            break;
        }
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
    //get the index of the tile the projectile is in
    int curTile = (int)(mCollisionBox.y / CTile::TILE_HEIGHT) * (levelDimensions[save.level].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH);
    //checks the current tile
    if ((tiles[curTile]->getType() > TILE_EMPTY || tiles[curTile]->getType() == TILE_GHOST_T_D || tiles[curTile]->getType() == TILE_GHOST_F_D) //ensures the tile to check is collidable
        && checkCollision(mCollisionBox, tiles[curTile]->getBox())) return true;
    //checks the tile to the right
    if (curTile + 1 < tileCount //ensures the tile to the right of the projectile exists
        && (tiles[curTile + 1]->getType() > TILE_EMPTY || tiles[curTile + 1]->getType() == TILE_GHOST_T_D || tiles[curTile + 1]->getType() == TILE_GHOST_F_D) //ensures the tile to check is collidable
        && checkCollision(mCollisionBox, tiles[curTile + 1]->getBox())) return true;
    //checks the tile below
    if (curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH < tileCount //ensures the tile below the projectile exists
        && (tiles[curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH]->getType() > TILE_EMPTY || tiles[curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH]->getType() == TILE_GHOST_T_D || tiles[curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH]->getType() == TILE_GHOST_F_D) //ensures the tile to check is collidable
        && checkCollision(mCollisionBox, tiles[curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH]->getBox())) return true;
    //checks the tile to the bottom right
    if (curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH + 1 < tileCount //ensures the tile to the bottom right of the projectile exists
        && (tiles[curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH + 1]->getType() > TILE_EMPTY || tiles[curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH + 1]->getType() == TILE_GHOST_T_D || tiles[curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH + 1]->getType() == TILE_GHOST_F_D) //ensures the tile to check is collidable
        && checkCollision(mCollisionBox, tiles[curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH + 1]->getBox())) return true;
    return false;
}
bool CProjectile::touchesGround()
{
    //check for if the projectile is at the bottom of the level
    if (mCollisionBox.y == levelDimensions[save.level].h - mCollisionBox.h) return true;
    SDL_Rect groundBox = {mCollisionBox.x, mCollisionBox.y + mCollisionBox.h, mCollisionBox.w, 1};
    //gets the index of the tile directly below the projectile
    int bottomTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) + 1) * (levelDimensions[save.level].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH);
    //checks the tile below
    if (bottomTile < tileCount //ensures the tile below the projectile exists
        && (tiles[bottomTile]->getType() > TILE_EMPTY || tiles[bottomTile]->getType() == TILE_GHOST_T_D || tiles[bottomTile]->getType() == TILE_GHOST_F_D) //ensures the tile to check is collidable
        && checkCollision(groundBox, tiles[bottomTile]->getBox())) return true;
    //checks the tile to the bottom right
    if (bottomTile + 1 < tileCount //ensures the tile to the bottom right of the projectile exists
        && (tiles[bottomTile + 1]->getType() > TILE_EMPTY || tiles[bottomTile + 1]->getType() == TILE_GHOST_T_D || tiles[bottomTile + 1]->getType() == TILE_GHOST_F_D) //ensures the tile to check is collidable
        && checkCollision(groundBox, tiles[bottomTile + 1]->getBox())) return true;
    return false;
}
bool CProjectile::touchesCeiling()
{
    //check for if the projectile is at the top of the level
    if (mCollisionBox.y == 0) return true; 
    SDL_Rect ceilingBox = {mCollisionBox.x, mCollisionBox.y - 1, mCollisionBox.w, 1};
    //gets the index of the tile directly above the projectile
    int topTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH);
    //checks the tile above
    if (topTile >= 0 //ensures the tile above the projectile exists
        && (tiles[topTile]->getType() > TILE_EMPTY || tiles[topTile]->getType() == TILE_GHOST_T_D || tiles[topTile]->getType() == TILE_GHOST_F_D) //ensures the tile to check is collidable
        && checkCollision(ceilingBox, tiles[topTile]->getBox())) return true;
    //checks the tile to the top right
    if (topTile + 1 >= 0 //ensures the tile to the top right of the projectile exists
        && (tiles[topTile + 1]->getType() > TILE_EMPTY || tiles[topTile + 1]->getType() == TILE_GHOST_T_D || tiles[topTile + 1]->getType() == TILE_GHOST_F_D) //ensures the tile to check is collidable
        && checkCollision(ceilingBox, tiles[topTile + 1]->getBox())) return true;
    return false;
}
SDL_Point CProjectile::getNearestCollision(int xVel, int yVel, SDL_Rect oldBox)
{
    SDL_Point point = {mCollisionBox.x, mCollisionBox.y};
    int curTile = (int)(mCollisionBox.y / CTile::TILE_HEIGHT) * (levelDimensions[save.level].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH);
    for (int i = 0; i < 2; i++) {
        curTile += i * levelDimensions[save.level].w / CTile::TILE_WIDTH;
        for (int j = curTile; j < curTile + 2; j++) {
            if (j < 0 || j >= tileCount) continue;
            if ((tiles[j]->getType() > TILE_EMPTY || tiles[j]->getType() == TILE_GHOST_T_D || tiles[j]->getType() == TILE_GHOST_F_D) && checkCollision(mCollisionBox, tiles[j]->getBox())) {
                //if the projectile used to be in the vertical range of the tile at all but is moving to the right, the position is set back to the left of the tile
                if(xVel > 0 && oldBox.y > tiles[j]->getBox().y - oldBox.h && oldBox.y < tiles[j]->getBox().y + tiles[j]->getBox().h) point.x = tiles[j]->getBox().x - oldBox.w;
                //if the projectile used to be in the vertical range of the tile at all but is moving to the left, the position is set back to the right of the tile
                else if(xVel < 0 && oldBox.y > tiles[j]->getBox().y - oldBox.h && oldBox.y < tiles[j]->getBox().y + tiles[j]->getBox().h) point.x = tiles[j]->getBox().x + tiles[i]->getBox().w;
                //if the projectile used to be in the horizontal range of the tile at all but is moving downwards, the position is set back to the top of the tile
                if(yVel > 0 && oldBox.x > tiles[j]->getBox().x - oldBox.w && oldBox.x < tiles[j]->getBox().x + tiles[j]->getBox().w) point.y = tiles[j]->getBox().y - oldBox.h;
                //if the projectile used to be in the horizontal range of the tile at all but is moving upwards, the position is set back to the bottom of the tile
                else if(yVel < 0 && oldBox.x > tiles[j]->getBox().x - oldBox.w && oldBox.x < tiles[j]->getBox().x + tiles[j]->getBox().w) point.y = tiles[j]->getBox().y + tiles[i]->getBox().h;
            }
        }
    }
    return point;
}