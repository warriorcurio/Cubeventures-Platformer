#include "CPlayer.h"
#include "Game.h"

const float coyoteTimeSeconds = 0.1f;
const float safePositionTimeSeconds = 5.f;
const float invulnerabilityTimeSeconds = 1.f;
const float rainbowTimeSeconds = 10.f;
float coyoteTimeTimerSeconds;
float safePositionTimeTimerSeconds;
float invulnerabilityTimeTimerSeconds;
float rainbowTimeTimerSeconds;

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
SDL_Rect rainbowAnimFrameClips[6] = {
    { 0, 40, 20, 20},
    {20, 40, 20, 20},
    {40, 40, 20, 20},
    { 0, 60, 20, 20},
    {20, 60, 20, 20},
    {40, 60, 20, 20}
};

CPlayer::CPlayer(int x, int y)
{
    mTexture.loadFromFile("res/player.png");
    mFrame = 0;
    mHealth = save.curHealth;
    mShield = 0;
    mSafePos = {x, y};
    mCollisionBox = {x, y, PLAYER_WIDTH, PLAYER_HEIGHT};
    mPlayerVel = -1;
    mVelX = 0;
    mVelY = 0;
    mJumpsRemaining = 0;
    mMaxJumps = save.maxJumps;
    setForm(save.form);
    setKeys(save.keys);
    mCharge = 0;
    mIsClimbing = false;
    mIsInvulnerable = false;
    mIsOnGround = false;
}
CPlayer::~CPlayer()
{
    mTexture.free();
}
void CPlayer::handleEvent(SDL_Event* e)
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
        if (e->key.keysym.sym == keybinds[KEYBINDS_UP] && (mIsClimbing || mForm == FORM_RAINBOW)) mVelY -= mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_LEFT]) mVelX -= mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_DOWN] && (mIsClimbing || mForm == FORM_RAINBOW)) mVelY += mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_RIGHT]) mVelX += mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_JUMP] && !mIsClimbing && mJumpsRemaining > 0 && mForm != FORM_RAINBOW) {
            mVelY = -mJumpVelMax;
            mJumpsRemaining--;
        }
        switch (e->key.keysym.sym) {
            case SDLK_p:
                setForm((mForm + 1) % FORMS_TOTAL);
                break;
        }
    } else if (e->type == SDL_KEYUP && e->key.repeat == 0) {
        if (e->key.keysym.sym == keybinds[KEYBINDS_UP] && (mIsClimbing || mForm == FORM_RAINBOW)) mVelY = 0;
        if (e->key.keysym.sym == keybinds[KEYBINDS_LEFT]) mVelX += mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_DOWN] && (mIsClimbing || mForm == FORM_RAINBOW)) mVelY = 0;
        if (e->key.keysym.sym == keybinds[KEYBINDS_RIGHT]) mVelX -= mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_JUMP] && !mIsClimbing && mVelY < -mJumpVelMin && mForm != FORM_RAINBOW) mVelY = -mJumpVelMin;
    }
}
void CPlayer::move(float timeStep)
{
    SDL_Rect tempBox = mCollisionBox;
    if (mIsInvulnerable) invulnerabilityTimeTimerSeconds += timeStep;
    if (invulnerabilityTimeTimerSeconds > invulnerabilityTimeSeconds) {
        mIsInvulnerable = false;
        invulnerabilityTimeTimerSeconds = 0;
    }
    if (mForm == FORM_RAINBOW) rainbowTimeTimerSeconds += timeStep;
    if (rainbowTimeTimerSeconds > rainbowTimeSeconds) {
        mIsInvulnerable = true;
        setForm(FORM_WHITE);
        rainbowTimeTimerSeconds = 0;
    }
    mCollisionBox.x += mVelX * timeStep;
    if(mCollisionBox.x < 0) mCollisionBox.x = 0;
    else if(mCollisionBox.x > levelDimensions[save.level - 1].w - PLAYER_WIDTH) mCollisionBox.x = levelDimensions[save.level - 1].w - PLAYER_WIDTH;
    mCollisionBox.y += mVelY * timeStep;
    if (mGravity > 0 && mVelY > 6 * mGravity) mVelY = 6 * mGravity;
    if(mCollisionBox.y < 0) mCollisionBox.y = 0;
    else if(mCollisionBox.y > levelDimensions[save.level - 1].h - PLAYER_HEIGHT) mCollisionBox.y = levelDimensions[save.level - 1].h - PLAYER_HEIGHT;
    if (touchesTile()) {
        SDL_Point point = getNearestCollision(mVelX, 0, tempBox);
        mCollisionBox.x = point.x;
        point = getNearestCollision(0, mVelY, tempBox);
        mCollisionBox.y = point.y;  
    }
    if (!mIsClimbing && mForm == FORM_BLUE && (touchesWallLeft() || touchesWallRight())) {
        mIsClimbing = true;
        mVelY = 0;
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_GetScancodeFromKey(keybinds[KEYBINDS_UP])] || SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_LEFTY) < -JOYSTICK_DEAD_ZONE) mVelY -= mPlayerVel;
        if (currentKeyStates[SDL_GetScancodeFromKey(keybinds[KEYBINDS_DOWN])] || SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_LEFTY) > JOYSTICK_DEAD_ZONE) mVelY += mPlayerVel;
    } else if (!(mForm == FORM_BLUE && (touchesWallLeft() || touchesWallRight())) ){
        mIsClimbing = false;
        mVelY += mGravity * timeStep;
    }
    if (touchesGround()) {
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
    if (touchesCeiling() && mVelY < 0) mVelY = 0;
}
void CPlayer::setCamera(SDL_Rect& camera)
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
void CPlayer::checkSpecialTileCollisions()
{
    int topLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topLeftTile + i * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH);
        if (curTile < 0 || curTile + 2 >= tileCount) continue;
        if(tiles[curTile]->getType() < TILE_EMPTY && checkCollision(mCollisionBox, tiles[curTile]->getBox())) tiles[curTile]->collisionEvent(curTile);
        if(tiles[curTile + 1]->getType() < TILE_EMPTY && checkCollision(mCollisionBox, tiles[curTile + 1]->getBox())) tiles[curTile + 1]->collisionEvent(curTile);
        if(tiles[curTile + 2]->getType() < TILE_EMPTY && checkCollision(mCollisionBox, tiles[curTile + 2]->getBox())) tiles[curTile + 2]->collisionEvent(curTile);
    }
}
void CPlayer::setForm(int form)
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
        case FORM_RAINBOW:
            mTexture.setColour(0xFF, 0xFF, 0xFF);
            mPlayerVel = 450;
            mGravity = 0;
            mJumpVelMax = 0;
            mJumpVelMin = 0;
            mVelY = 0;
            break;
    }
    mVelX = mPlayerVel * modifiedVel;
    save.form = mForm;
}
void CPlayer::setCustomForm(int playerVel, int jumpMax, int jumpMin, int gravity)
{
    int modifiedVel = 0;
    if (mVelX == mPlayerVel || mVelX == -mPlayerVel) {
        modifiedVel = mVelX == mPlayerVel ? 1 : -1;
        mVelX = 0;
    }
    mPlayerVel = playerVel;
    mJumpVelMax = jumpMax;
    mJumpVelMin = jumpMin;
    mGravity = gravity;
    mVelX = mPlayerVel * modifiedVel;
    save.form = mForm;
}
void CPlayer::setHealth(int health)
{
    mHealth = health;
    save.curHealth = mHealth;
}
void CPlayer::setShield(int shield)
{
    mShield = shield;
}
void CPlayer::setCharge(int charge)
{
    mCharge = charge;
    if (charge == 100) {
        mCharge = 0;
        setForm(FORM_RAINBOW);
        mFrame = 0;
    }
}
void CPlayer::setJumps(int jumps)
{
    mJumpsRemaining = jumps;
}
void CPlayer::setKeys(int keys)
{
    mKeys = keys;
    save.keys = mKeys;
}
void CPlayer::setPos(int x, int y)
{
    mCollisionBox.x = x;
    mCollisionBox.y = y;
    mSafePos.x = x;
    mSafePos.y = y;
    safePositionTimeTimerSeconds = 0.f;
    save.x = x;
    save.y = y;
}
void CPlayer::setVel(int velX, int velY)
{
    mVelX = velX;
    mVelY = velY;
}
void CPlayer::setInvulnerable(bool isInvulnerable)
{
    mIsInvulnerable = isInvulnerable;
}
void CPlayer::render(SDL_Rect& camera)
{
    if (mForm != FORM_RAINBOW) {
        int frameSpeed = 3 + (int)((100 - mCharge) * 12 / 100);
        if (!mIsInvulnerable) mFrame = (mFrame + 1) % (frameSpeed * 11);
        if (!mIsInvulnerable || rand() % 3 == 0) mTexture.render((int)mCollisionBox.x - camera.x, (int)mCollisionBox.y - camera.y, &defaultAnimFrameClips[mFrame / frameSpeed]);
    } else {
        mTexture.setColour(0xFF - 0x7F * rainbowTimeTimerSeconds / rainbowTimeSeconds, 0xFF - 0x7F * rainbowTimeTimerSeconds / rainbowTimeSeconds, 0xFF - 0x7F * rainbowTimeTimerSeconds / rainbowTimeSeconds);
        mFrame = (mFrame + 1) % 18;
        mTexture.render((int)mCollisionBox.x - camera.x, (int)mCollisionBox.y - camera.y, &rainbowAnimFrameClips[mFrame / 3]);
    }
}
int CPlayer::getForm()
{
    return mForm;
}
int CPlayer::getPlayerVel()
{
    return mPlayerVel;
}
int CPlayer::getJumpMax()
{
    return mJumpVelMax;
}
int CPlayer::getJumpMin()
{
    return mJumpVelMin;
}
int CPlayer::getGravity()
{
    return mGravity;
}
int CPlayer::getHealth()
{
    return mHealth;
}
int CPlayer::getShield()
{
    return mShield;
}
int CPlayer::getCharge()
{
    return mCharge;
}
SDL_Point CPlayer::getSafePos()
{
    return mSafePos;
}
SDL_Rect CPlayer::getBox()
{
    return mCollisionBox;
}
int CPlayer::getJumps()
{
    return mJumpsRemaining;
}
int CPlayer::getKeys()
{
    return mKeys;
}
int CPlayer::getPosX()
{
    return mCollisionBox.x;
}
int CPlayer::getPosY()
{
    return mCollisionBox.y;
}
int CPlayer::getVelX()
{
    return mVelX;
}
int CPlayer::getVelY()
{
    return mVelY;
}
bool CPlayer::getInvulnerable()
{
    return mIsInvulnerable;
}
bool CPlayer::touchesTile()
{
    int topLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topLeftTile + i * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH);
        if (curTile < 0 || curTile + 2 >= tileCount) continue;
        if(tiles[curTile]->getType() > TILE_EMPTY && checkCollision(mCollisionBox, tiles[curTile]->getBox())) return true;
        if(tiles[curTile + 1]->getType() > TILE_EMPTY && checkCollision(mCollisionBox, tiles[curTile + 1]->getBox())) return true;
        if(tiles[curTile + 2]->getType() > TILE_EMPTY && checkCollision(mCollisionBox, tiles[curTile + 2]->getBox())) return true;
    }
    return false;
}
bool CPlayer::touchesGround()
{
    if (mCollisionBox.y == levelDimensions[save.level - 1].h - mCollisionBox.h) return true; 
    SDL_Rect groundBox = {mCollisionBox.x, mCollisionBox.y + mCollisionBox.h, mCollisionBox.w, 1};
    int bottomLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) + 1) * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = bottomLeftTile; i < bottomLeftTile + 3; i++) {
        if (i >= tileCount) continue;
        if(tiles[i]->getType() > TILE_EMPTY && checkCollision(groundBox, tiles[i]->getBox())) return true;
    }
    return false;
}
bool CPlayer::touchesCeiling()
{
    if (mCollisionBox.y == 0) return true; 
    SDL_Rect ceilingBox = {mCollisionBox.x, mCollisionBox.y - 1, mCollisionBox.w, 1};
    int topLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = topLeftTile; i < topLeftTile + 3; i++) {
        if (i < 0) continue;
        if(tiles[i]->getType() > TILE_EMPTY && checkCollision(ceilingBox, tiles[i]->getBox())) return true;
    }
    return false;
}
bool CPlayer::touchesWallRight()
{
    if (mCollisionBox.x == levelDimensions[save.level - 1].w - mCollisionBox.w) return true; 
    SDL_Rect rightBox = {mCollisionBox.x + mCollisionBox.w, mCollisionBox.y, 1, mCollisionBox.h};
    int topRightTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) + 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topRightTile + i * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH);
        if (curTile < 0 || curTile >= tileCount) continue;
        if(tiles[curTile]->getType() > TILE_EMPTY && tiles[curTile]->getType() != TILE_GHOST_T && checkCollision(rightBox, tiles[curTile]->getBox())) return true;
    }
    return false;
}
bool CPlayer::touchesWallLeft()
{
    if (mCollisionBox.x == 0) return true; 
    SDL_Rect leftBox = {mCollisionBox.x - 1, mCollisionBox.y, 1, mCollisionBox.h};
    int topLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topLeftTile + i * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH);
        if (curTile < 0 || curTile >= tileCount) continue;
        if(tiles[curTile]->getType() > TILE_EMPTY && tiles[curTile]->getType() != TILE_GHOST_T && checkCollision(leftBox, tiles[curTile]->getBox())) return true;
    }
    return false;
}
SDL_Point CPlayer::getNearestCollision(int xVel, int yVel, SDL_Rect oldBox)
{
    SDL_Point point = {mCollisionBox.x, mCollisionBox.y};
    int topLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topLeftTile + i * (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH);
        for (int j = curTile; j < curTile + 3; j++) {
            if (j < 0 || j > tileCount) continue;
            if (tiles[j]->getType() > TILE_EMPTY && checkCollision(mCollisionBox, tiles[j]->getBox())) {
                if(xVel > 0 && oldBox.y > tiles[j]->getBox().y - oldBox.h && oldBox.y < tiles[j]->getBox().y + tiles[j]->getBox().h) point.x = tiles[j]->getBox().x - oldBox.w;
                else if(xVel < 0 && oldBox.y > tiles[j]->getBox().y - oldBox.h && oldBox.y < tiles[j]->getBox().y + tiles[j]->getBox().h) point.x = tiles[j]->getBox().x + tiles[j]->getBox().w;
                if(yVel > 0 && oldBox.x > tiles[j]->getBox().x - oldBox.w && oldBox.x < tiles[j]->getBox().x + tiles[j]->getBox().w) point.y = tiles[j]->getBox().y - oldBox.h;
                else if(yVel < 0 && oldBox.x > tiles[j]->getBox().x - oldBox.w && oldBox.x < tiles[j]->getBox().x + tiles[j]->getBox().w) point.y = tiles[j]->getBox().y + tiles[j]->getBox().h;
            }
        }
    }
    return point;
}