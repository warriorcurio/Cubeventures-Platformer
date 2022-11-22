#include "LTile.h"
#include "LPlayer.h"

LTile::LTile(int x, int y, int tileType)
{
    mCollisionBox.x = x;
    mCollisionBox.y = y;
    mCollisionBox.w = TILE_WIDTH;
    mCollisionBox.h = TILE_HEIGHT;
    mType = tileType;
}
void LTile::collisionEvent()
{
    switch (mType) {
        case TILE_WHITECRYSTAL:
            mType = TILE_EMPTY;
            player->setForm(FORM_WHITE);
            break;
        case TILE_REDCRYSTAL:
            mType = TILE_EMPTY;
            player->setForm(FORM_RED);
            break;
        case TILE_GREENCRYSTAL:
            mType = TILE_EMPTY;
            player->setForm(FORM_GREEN);
            break;
        case TILE_BLUECRYSTAL:
            mType = TILE_EMPTY;
            player->setForm(FORM_BLUE);
            break;
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