#include "LTile.h"
#include "LPlayer.h"

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
void LTile::collisionEvent()
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
    }
}
void LTile::updateTimers(float timeStep)
{
    if (mActivationTime == 0) return;
    mActivationCounter += timeStep;
    if (mActivationCounter < mActivationTime) return;
    mActivationCounter = 0;
    mActivationTime = 0;
    switch (mType) {
        case TILE_WHITECRYSTAL_D: mType = TILE_WHITECRYSTAL; break;
        case TILE_REDCRYSTAL_D: mType = TILE_REDCRYSTAL; break;
        case TILE_GREENCRYSTAL_D: mType = TILE_GREENCRYSTAL; break;
        case TILE_BLUECRYSTAL_D: mType = TILE_BLUECRYSTAL; break;
    }
}
void LTile::render(SDL_Rect& camera)
{
    if(checkCollision(camera, mCollisionBox))
    {
        tileTexture.render(mCollisionBox.x - camera.x, mCollisionBox.y - camera.y, &tileClips[mType]);
    }
}
int LTile::getType()
{
    return mType;
}
SDL_Rect LTile::getBox()
{
    return mCollisionBox;
}