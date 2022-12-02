#include "LTile.h"
#include "LPlayer.h"
#include "Game.h"

LTile::LTile(int x, int y, int tileType)
{
    mCollisionBox.x = x;
    mCollisionBox.y = y;
    mCollisionBox.w = TILE_WIDTH;
    mCollisionBox.h = TILE_HEIGHT;
    mActivationTime = 0;
    mActivationCounter = 0;
    mType = tileType;
}
void LTile::collisionEvent(int tileNum)
{
    switch (mType) {
        case TILE_WHITECRYSTAL:
            mType = TILE_WHITECRYSTAL_D;
            mActivationTime = 2.f;
            player->setForm(FORM_WHITE);
            break;
        case TILE_REDCRYSTAL:
            mType = TILE_REDCRYSTAL_D;
            mActivationTime = 2.f;
            player->setForm(FORM_RED);
            break;
        case TILE_GREENCRYSTAL:
            mType = TILE_GREENCRYSTAL_D;
            mActivationTime = 2.f;
            player->setForm(FORM_GREEN);
            break;
        case TILE_BLUECRYSTAL:
            mType = TILE_BLUECRYSTAL_D;
            mActivationTime = 2.f;
            player->setForm(FORM_BLUE);
            break;
        case TILE_JUMPCRYSTAL:
            mType = TILE_JUMPCRYSTAL_D;
            mActivationTime = 2.5f;
            player->setJumps(save.maxJumps);
            break;
        case TILE_EXIT:
            nextLevel();
            break;
        case TILE_KEY:
            mType = TILE_EMPTY;
            player->setKeys(player->getKeys() + 1);
            for (int i = 0; i < 5; i++) {
                if (!save.collectedKeys[i]) {
                    save.collectedKeys[i] = tileNum;
                    break;
                }
            }
            break;
        case TILE_LOCK_D:
            mType = TILE_EMPTY;
            player->setKeys(player->getKeys() - 1);
            if (player->getKeys() < 0) {
                mType = TILE_LOCK;
                player->setKeys(0);
                break;
            }
            for (int i = 0; i < 5; i++) {
                if (!save.unlockedLocks[i]) {
                    save.unlockedLocks[i] = tileNum;
                    break;
                }
            }
            break;
        case TILE_SPIKE:
            player->setHealth(player->getHealth() - 1);
            player->setPos(player->getSafePos().x, player->getSafePos().y);
            if (player->getHealth() == 0) {
                transition(SCENE_MAINMENU);
            }
            break;
    }
}
void LTile::updateTiles(float timeStep)
{
    if (mActivationTime != 0) {
        mActivationCounter += timeStep;
        if (mActivationCounter < mActivationTime) return;
        mActivationCounter = 0;
        mActivationTime = 0;
        switch (mType) {
            case TILE_WHITECRYSTAL_D: mType = TILE_WHITECRYSTAL; break;
            case TILE_REDCRYSTAL_D: mType = TILE_REDCRYSTAL; break;
            case TILE_GREENCRYSTAL_D: mType = TILE_GREENCRYSTAL; break;
            case TILE_BLUECRYSTAL_D: mType = TILE_BLUECRYSTAL; break;
            case TILE_JUMPCRYSTAL_D: mType = TILE_JUMPCRYSTAL; break;
        }
    } else {
        switch (mType) {
            case TILE_GHOST: if (player->getForm() == FORM_WHITE) mType = TILE_GHOST_D; break;
            case TILE_GHOST_D: if (player->getForm() != FORM_WHITE) mType = TILE_GHOST; break;
            case TILE_LOCK: if (player->getKeys() > 0) mType = TILE_LOCK_D; break;
            case TILE_LOCK_D: if (player->getKeys() == 0) mType = TILE_LOCK; break;
        }
    }
}
void LTile::render(SDL_Rect& camera)
{
    if(checkCollision(camera, mCollisionBox))
    {
        tileTexture.render(mCollisionBox.x - camera.x, mCollisionBox.y - camera.y, &tileClips[mType]);
    }
}
void LTile::setType(int type)
{
    mType = type;
}
int LTile::getType()
{
    return mType;
}
SDL_Rect LTile::getBox()
{
    return mCollisionBox;
}