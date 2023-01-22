#include "LPlayer.h"
#include "Game.h"

const float coyoteTimeSeconds = 0.1f;
const float safePositionTimeSeconds = 5.f;
const float invulnerabilityTimeSeconds = 1.f;
float coyoteTimeTimerSeconds;
float safePositionTimeTimerSeconds;
float invulnerabilityTimeTimerSeconds;

SDL_Rect defaultAnimFrameClips[11] = {
    { 0,  0, 20, 20},
    {20,  0, 20, 20},
    {40,  0, 20, 20},
    { 0, 20, 20, 20},
    {20, 20, 20, 20},
    {40, 20, 20, 20},
    {20, 20, 20, 20},
    { 0, 20, 20, 20},
    {40,  0, 20, 20},
    {20,  0, 20, 20},
    { 0,  0, 20, 20}
};

LPlayer::LPlayer(int x, int y)
{
    mTexture.loadFromFile("res/player.png");
    mFrame = 0;
    mHealth = save.curHealth;
    mSafePos = {x, y};
    mCollisionBox = {x, y, PLAYER_WIDTH, PLAYER_HEIGHT};
    mPlayerVel = -1;
    mVelX = 0;
    mVelY = 0;
    mJumpsRemaining = 0;
    mMaxJumps = save.maxJumps;
    setForm(save.form);
    setKeys(save.keys);
    mIsClimbing = false;
    mIsInvulnerable = false;
    mIsOnGround = false;
}
LPlayer::~LPlayer()
{
    mTexture.free();
}
void LPlayer::handleEvent(SDL_Event* e)
{
    if (e->type == SDL_JOYAXISMOTION && e->jaxis.which == 0) {
        if (e->jaxis.axis == 0) {
            if (e->jaxis.value > JOYSTICK_DEAD_ZONE) mVelX = mPlayerVel;
            else if (e->jaxis.value < -JOYSTICK_DEAD_ZONE) mVelX = -mPlayerVel;
            else mVelX = 0;
        } else if (e->jaxis.axis == 1 && mIsClimbing) {
            if (e->jaxis.value > JOYSTICK_DEAD_ZONE) mVelY = mPlayerVel;
            else if (e->jaxis.value < -JOYSTICK_DEAD_ZONE) mVelY = -mPlayerVel;
            else mVelY = 0;
        }
    } else if (e->type == SDL_JOYBUTTONDOWN) {
        if (e->jbutton.button == SDL_CONTROLLER_BUTTON_A && !mIsClimbing && mJumpsRemaining > 0) {
            mVelY = -mJumpVelMax;
            mJumpsRemaining--;
            SDL_GameControllerRumble(gController, 0x00FF, 0x00FF, 50);
        } 
    } else if (e->type == SDL_JOYBUTTONUP) {
        if (e->jbutton.button == SDL_CONTROLLER_BUTTON_A && !mIsClimbing && mVelY < -mJumpVelMin) {
            mVelY = -mJumpVelMin;
        }
    }
    if (gController) return;
    if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
        if (e->key.keysym.sym == keybinds[KEYBINDS_UP] && mIsClimbing) mVelY -= mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_LEFT]) mVelX -= mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_DOWN] && mIsClimbing) mVelY += mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_RIGHT]) mVelX += mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_JUMP] && !mIsClimbing && mJumpsRemaining > 0) {
            mVelY = -mJumpVelMax;
            mJumpsRemaining--;
        }
        switch (e->key.keysym.sym) {
            case SDLK_p:
                setForm((mForm + 1) % FORMS_TOTAL);
                break;
        }
    } else if (e->type == SDL_KEYUP && e->key.repeat == 0) {
        if (e->key.keysym.sym == keybinds[KEYBINDS_UP] && mIsClimbing) mVelY = 0;
        if (e->key.keysym.sym == keybinds[KEYBINDS_LEFT]) mVelX += mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_DOWN] && mIsClimbing) mVelY = 0;
        if (e->key.keysym.sym == keybinds[KEYBINDS_RIGHT]) mVelX -= mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_JUMP] && !mIsClimbing && mVelY < -mJumpVelMin) mVelY = -mJumpVelMin;
    }
}
void LPlayer::move(std::vector<LTile*>& tiles, float timeStep)
{
    SDL_Rect tempBox = mCollisionBox;
    if (mIsInvulnerable) invulnerabilityTimeTimerSeconds += timeStep;
    if (invulnerabilityTimeTimerSeconds > invulnerabilityTimeSeconds) {
        mIsInvulnerable = false;
        invulnerabilityTimeTimerSeconds = 0;
    }
    mCollisionBox.x += mVelX * timeStep;
    if(mCollisionBox.x < 0) mCollisionBox.x = 0;
    else if(mCollisionBox.x > levelDimensions[save.level - 1].w - PLAYER_WIDTH) mCollisionBox.x = levelDimensions[save.level - 1].w - PLAYER_WIDTH;
    if (!mIsClimbing && mForm == FORM_BLUE && (touchesWallLeft(tiles) || touchesWallRight(tiles))) {
        mIsClimbing = true;
        mVelY = 0;
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_GetScancodeFromKey(keybinds[KEYBINDS_UP])] || SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_LEFTY) < -JOYSTICK_DEAD_ZONE) mVelY -= mPlayerVel;
        if (currentKeyStates[SDL_GetScancodeFromKey(keybinds[KEYBINDS_DOWN])] || SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_LEFTY) > JOYSTICK_DEAD_ZONE) mVelY += mPlayerVel;
    } else if (!(mForm == FORM_BLUE && (touchesWallLeft(tiles) || touchesWallRight(tiles))) ){
        mIsClimbing = false;
        mVelY += mGravity * timeStep;
    }
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
        mIsOnGround = true;
        mJumpsRemaining = mMaxJumps;
        safePositionTimeTimerSeconds += timeStep;
        if (safePositionTimeTimerSeconds >= safePositionTimeSeconds && !mIsInvulnerable) {
            mSafePos = {mCollisionBox.x, mCollisionBox.y};
            safePositionTimeTimerSeconds = 0;
            if (save.difficulty == DIFFICULTY_EASY && mHealth < save.maxHealth) mHealth++;
            save.x = mCollisionBox.x;
            save.y = mCollisionBox.y;
        }
    } else if (mIsOnGround) {
        coyoteTimeTimerSeconds += timeStep;
        if (coyoteTimeTimerSeconds >= coyoteTimeSeconds) {
            coyoteTimeTimerSeconds = 0;
            mIsOnGround = false;
            if (mJumpsRemaining == mMaxJumps) mJumpsRemaining--;
        }
    }
    if (touchesCeiling(tiles) && mVelY < 0) mVelY = 0;
}
void LPlayer::setCamera(SDL_Rect& camera)
{
    SDL_Rect tempCamera = camera;
    camera.x = ((int)mCollisionBox.x + PLAYER_WIDTH / 2) - LOGICAL_SCREEN_WIDTH / 2;
    camera.y = ((int)mCollisionBox.y + PLAYER_HEIGHT / 2) - LOGICAL_SCREEN_HEIGHT / 2;
    camera.w = LOGICAL_SCREEN_WIDTH;
    camera.h = LOGICAL_SCREEN_HEIGHT;
    if(camera.x < 0) camera.x = 0;
    if(camera.y < 0) camera.y = 0;
    if(camera.x > levelDimensions[save.level - 1].w - camera.w) camera.x = levelDimensions[save.level - 1].w - camera.w;
    if(camera.y > levelDimensions[save.level - 1].h - camera.h) camera.y = levelDimensions[save.level - 1].h - camera.h;
    parallaxOffset -= (camera.x - tempCamera.x) / 3;
}
void LPlayer::checkItemCollisions(std::vector<LTile*>& tiles)
{
    for(int i = 0; i < tileCount; i++)
    {
        if(tiles[i]->getType() < TILE_EMPTY && checkCollision(mCollisionBox, tiles[i]->getBox()))
        {
            tiles[i]->collisionEvent(i);
        }
    }
}
void LPlayer::setForm(int form)
{
    int modifiedVel = 0;
    if (mVelX == mPlayerVel || mVelX == -mPlayerVel) {
        modifiedVel = mVelX == mPlayerVel ? 1 : -1;
        mVelX = 0;
    }
    mForm = form;
    switch (form) {
        case FORM_WHITE:
            mTexture.setColour(0xFF, 0xFF, 0xFF);
            mPlayerVel = 300;
            mGravity = 1680;
            mJumpVelMax = 840;
            mJumpVelMin = 380;
            break;
        case FORM_RED:
            mTexture.setColour(0xFF, 0x00, 0x00);
            mPlayerVel = 450;
            mGravity = 1680;
            mJumpVelMax = 840;
            mJumpVelMin = 380;
            break;
        case FORM_GREEN:
            mTexture.setColour(0x00, 0xFF, 0x00);
            mPlayerVel = 300;
            mGravity = 2520;
            mJumpVelMax = 1260;
            mJumpVelMin = 570;
            break;
        case FORM_BLUE:
            mTexture.setColour(0x00, 0x00, 0xFF);
            mPlayerVel = 225;
            mGravity = 1260;
            mJumpVelMax = 630;
            mJumpVelMin = 285;
            break;
    }
    mVelX = mPlayerVel * modifiedVel;
    save.form = mForm;
}
void LPlayer::setHealth(int health)
{
    mHealth = health;
    save.curHealth = mHealth;
}
void LPlayer::setJumps(int jumps)
{
    mJumpsRemaining = jumps;
}
void LPlayer::setKeys(int keys)
{
    mKeys = keys;
    save.keys = mKeys;
}
void LPlayer::setPos(int x, int y)
{
    mCollisionBox.x = x;
    mCollisionBox.y = y;
    safePositionTimeTimerSeconds = 0.f;
    save.x = mCollisionBox.x;
    save.y = mCollisionBox.y;
}
void LPlayer::setInvulnerable(bool isInvulnerable)
{
    mIsInvulnerable = isInvulnerable;
}
void LPlayer::render(SDL_Rect& camera)
{
    if (!mIsInvulnerable) mFrame = (mFrame + 1) % 43;
    if (!mIsInvulnerable || rand() % 3 == 0) mTexture.render((int)mCollisionBox.x - camera.x, (int)mCollisionBox.y - camera.y, &defaultAnimFrameClips[mFrame / 4]);
}
int LPlayer::getForm()
{
    return mForm;
}
int LPlayer::getHealth()
{
    return mHealth;
}
SDL_Point LPlayer::getSafePos()
{
    return mSafePos;
}
SDL_Rect LPlayer::getBox()
{
    return mCollisionBox;
}
int LPlayer::getJumps()
{
    return mJumpsRemaining;
}
int LPlayer::getKeys()
{
    return mKeys;
}
int LPlayer::getPosX()
{
    return (int)mCollisionBox.x;
}
int LPlayer::getPosY()
{
    return (int)mCollisionBox.y;
}
bool LPlayer::getInvulnerable()
{
    return mIsInvulnerable;
}
bool LPlayer::touchesTile(std::vector<LTile*>& tiles)
{
    int topLeftTile = ((int)(mCollisionBox.y / LTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH) + (int)(mCollisionBox.x / LTile::TILE_WIDTH) - 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topLeftTile + i * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH);
        if (curTile < 0 || curTile + 2 >= tileCount) continue;
        if(tiles[curTile]->getType() > TILE_EMPTY && checkCollision(mCollisionBox, tiles[curTile]->getBox())) return true;
        if(tiles[curTile + 1]->getType() > TILE_EMPTY && checkCollision(mCollisionBox, tiles[curTile + 1]->getBox())) return true;
        if(tiles[curTile + 2]->getType() > TILE_EMPTY && checkCollision(mCollisionBox, tiles[curTile + 2]->getBox())) return true;
    }
    return false;
}
bool LPlayer::touchesGround(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.y == levelDimensions[save.level - 1].h - mCollisionBox.h) return true; 
    SDL_Rect groundBox = {mCollisionBox.x, mCollisionBox.y + mCollisionBox.h, mCollisionBox.w, 1};
    int bottomLeftTile = ((int)(mCollisionBox.y / LTile::TILE_HEIGHT) + 1) * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH) + (int)(mCollisionBox.x / LTile::TILE_WIDTH) - 1;
    for (int i = bottomLeftTile; i < bottomLeftTile + 3; i++) {
        if (i >= tileCount) continue;
        if(tiles[i]->getType() > TILE_EMPTY && checkCollision(groundBox, tiles[i]->getBox())) return true;
    }
    return false;
}
bool LPlayer::touchesCeiling(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.y == 0) return true; 
    SDL_Rect ceilingBox = {mCollisionBox.x, mCollisionBox.y - 1, mCollisionBox.w, 1};
    int topLeftTile = ((int)(mCollisionBox.y / LTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH) + (int)(mCollisionBox.x / LTile::TILE_WIDTH) - 1;
    for (int i = topLeftTile; i < topLeftTile + 3; i++) {
        if (i < 0) continue;
        if(tiles[i]->getType() > TILE_EMPTY && checkCollision(ceilingBox, tiles[i]->getBox())) return true;
    }
    return false;
}
bool LPlayer::touchesWallRight(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.x == levelDimensions[save.level - 1].w - mCollisionBox.w) return true; 
    SDL_Rect rightBox = {mCollisionBox.x + mCollisionBox.w, mCollisionBox.y, 1, mCollisionBox.h};
    int topRightTile = ((int)(mCollisionBox.y / LTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH) + (int)(mCollisionBox.x / LTile::TILE_WIDTH) + 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topRightTile + i * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH);
        if (curTile < 0 || curTile >= tileCount) continue;
        if(tiles[curTile]->getType() > TILE_EMPTY && checkCollision(rightBox, tiles[curTile]->getBox())) return true;
    }
    return false;
}
bool LPlayer::touchesWallLeft(std::vector<LTile*>& tiles)
{
    if (mCollisionBox.x == 0) return true; 
    SDL_Rect leftBox = {mCollisionBox.x - 1, mCollisionBox.y, 1, mCollisionBox.h};
    int topLeftTile = ((int)(mCollisionBox.y / LTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH) + (int)(mCollisionBox.x / LTile::TILE_WIDTH) - 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topLeftTile + i * (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH);
        if (curTile < 0 || curTile >= tileCount) continue;
        if(tiles[curTile]->getType() > TILE_EMPTY && checkCollision(leftBox, tiles[curTile]->getBox())) return true;
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