#include "LTile.h"

LTile::LTile(int x, int y, int tileType)
{
    mCollisionBox.x = x;
    mCollisionBox.y = y;
    mCollisionBox.w = TILE_WIDTH;
    mCollisionBox.h = TILE_HEIGHT;
    mType = tileType;
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