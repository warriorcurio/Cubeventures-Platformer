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
    setForm(save.form);
    setKeys(save.keys);
    mCharge = save.charge;
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
    //controller input handling
    if (e->type == SDL_JOYAXISMOTION && e->jaxis.which == 0) {
        if (e->jaxis.axis == 0) {
            if (e->jaxis.value > JOYSTICK_DEAD_ZONE) mVelX = mPlayerVel;
            else if (e->jaxis.value < -JOYSTICK_DEAD_ZONE) mVelX = -mPlayerVel;
            else mVelX = 0;
        } else if (e->jaxis.axis == 1 && (mIsClimbing || mForm == FORM_RAINBOW)) {
            if (e->jaxis.value > JOYSTICK_DEAD_ZONE) mVelY = mPlayerVel;
            else if (e->jaxis.value < -JOYSTICK_DEAD_ZONE) mVelY = -mPlayerVel;
            else mVelY = 0;
        }
    } else if (e->type == SDL_JOYBUTTONDOWN) {
        if (e->jbutton.button == SDL_CONTROLLER_BUTTON_A && !mIsClimbing && mJumpsRemaining > 0) {
            mVelY = -mJumpVelMax;
            mJumpsRemaining--;
            Mix_PlayChannel(SFX_JUMP, sfx[SFX_JUMP], 0);
        } 
    } else if (e->type == SDL_JOYBUTTONUP) {
        if (e->jbutton.button == SDL_CONTROLLER_BUTTON_A && !mIsClimbing && mVelY < -mJumpVelMin) mVelY = -mJumpVelMin;
        else if (e->jbutton.button == SDL_CONTROLLER_BUTTON_Y && mCharge >= MAX_CHARGE) {
            setForm(FORM_RAINBOW);
            setShield(mShield + 1);
            setHealth(save.maxHealth);
            setCharge(0);
            mFrame = 0;
            char* temp = (char*)malloc(12);
            strncpy(temp, activity.GetDetails(), 12);
            std::string tempString = (std::string)temp;
            for (int i = 0; i < save.maxHealth + player->getShield(); i++) {
                if (i < save.maxHealth) tempString += "❤️";
                else tempString += "💙";
            }
            activity.SetDetails(tempString.c_str());
            core->ActivityManager().UpdateActivity(activity, NULL);
        }
    }
    if (gController) return; //ignores keyboard input when a controller is connected
    //keyboard input handling
    if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
        if (e->key.keysym.sym == keybinds[KEYBINDS_UP] && (mIsClimbing || mForm == FORM_RAINBOW)) mVelY -= mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_LEFT]) mVelX -= mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_DOWN] && (mIsClimbing || mForm == FORM_RAINBOW)) mVelY += mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_RIGHT]) mVelX += mPlayerVel;
        if (e->key.keysym.sym == keybinds[KEYBINDS_JUMP] && !mIsClimbing && mJumpsRemaining > 0 && mForm != FORM_RAINBOW) {
            mVelY = -mJumpVelMax;
            mJumpsRemaining--;
            Mix_PlayChannel(SFX_JUMP, sfx[SFX_JUMP], 0);
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
        if (e->key.keysym.sym == keybinds[KEYBINDS_JUMP] && !mIsClimbing && mVelY < -mJumpVelMin && mForm != FORM_RAINBOW) mVelY = -mJumpVelMin; //sets the vertical velocity lower if the player releases the jump button earlier
        if (e->key.keysym.sym == keybinds[KEYBINDS_ABILITY] && mCharge >= MAX_CHARGE) {
            setForm(FORM_RAINBOW);
            setShield(mShield + 1);
            setHealth(save.maxHealth);
            setCharge(0);
            mFrame = 0;
            char* temp = (char*)malloc(12);
            strncpy(temp, activity.GetDetails(), 12);
            std::string tempString = (std::string)temp;
            for (int i = 0; i < save.maxHealth + player->getShield(); i++) {
                if (i < save.maxHealth) tempString += "❤️";
                else tempString += "💙";
            }
            activity.SetDetails(tempString.c_str());
            core->ActivityManager().UpdateActivity(activity, NULL);
        }
    }
}
void CPlayer::move(float timeStep)
{
    SDL_Rect tempBox = mCollisionBox; //saves the position of the player before moving to aid in collision placement
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
        Mix_HaltChannel(SFX_RAINBOW);
    }
    mCollisionBox.x += (int)(mVelX * timeStep);
    if(mCollisionBox.x < 0) mCollisionBox.x = 0; //if the player goes off screen to the left, they are set back on screen
    else if(mCollisionBox.x > levelDimensions[save.level].w - PLAYER_WIDTH) mCollisionBox.x = levelDimensions[save.level].w - PLAYER_WIDTH; //if the player goes off screen to the right, they are set back on screen
    mCollisionBox.y += (int)(mVelY * timeStep);
    if (mVelY > 1200) mVelY = 1200;
    if(mCollisionBox.y < 0) mCollisionBox.y = 0; //if the player goes off screen above, they are set back on screen
    else if(mCollisionBox.y > levelDimensions[save.level].h - PLAYER_HEIGHT) mCollisionBox.y = levelDimensions[save.level].h - PLAYER_HEIGHT; //if the player goes off screen below, they are set back on screen
    if (touchesTile()) {
        //if the player has moved to within a tile, first the x position will be resolved while keeping the y position the same
        SDL_Point point = getNearestCollision(mVelX, 0, tempBox);
        mCollisionBox.x = point.x;
        //if the player is still within a tile when the x position has been resolved, the y position will then be resolved while keeping the x position the same
        point = getNearestCollision(0, mVelY, tempBox);
        mCollisionBox.y = point.y;  
    }
    if (!mIsClimbing && mForm == FORM_BLUE && touchesWall()) {
        mIsClimbing = true;
        mVelY = 0;
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_GetScancodeFromKey(keybinds[KEYBINDS_UP])] || SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_LEFTY) < -JOYSTICK_DEAD_ZONE) mVelY -= mPlayerVel;
        if (currentKeyStates[SDL_GetScancodeFromKey(keybinds[KEYBINDS_DOWN])] || SDL_GameControllerGetAxis(gController, SDL_CONTROLLER_AXIS_LEFTY) > JOYSTICK_DEAD_ZONE) mVelY += mPlayerVel;
    } else if (!(mForm == FORM_BLUE && touchesWall())){
        mIsClimbing = false;
        mCollisionBox.y += (int)(mGravity * timeStep * timeStep / 2);
        mVelY += (int)(mGravity * timeStep);
    }
    //restores jumps when on ground and stops vertical velocity
    //also increments the safe position timer, regenerating health and saving a new position when the timer has reached its maximum
    if (touchesGround()) {
        mVelY = 0;
        if (!mIsClimbing) mCollisionBox.y -= (int)(mGravity * timeStep * timeStep / 2);
        mIsOnGround = true;
        mJumpsRemaining = save.maxJumps;
        safePositionTimeTimerSeconds += timeStep;
        //ensures safe positions are accessible without invulnerability
        if (safePositionTimeTimerSeconds >= safePositionTimeSeconds && !mIsInvulnerable && mForm != FORM_RAINBOW) {
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
            if (mJumpsRemaining == save.maxJumps) mJumpsRemaining--;
        }
    }
    if (touchesCeiling() && mVelY < 0) mVelY = 0;
}
void CPlayer::setCamera(SDL_Rect& camera)
{
    SDL_Rect tempCamera = camera;
    //Positions the top left of the camera half a screen up and to the left of the player's centre
    //This means the player is in the centre of the camera
    camera.x = ((int)mCollisionBox.x + PLAYER_WIDTH / 2) - LOGICAL_SCREEN_WIDTH / 2;
    camera.y = ((int)mCollisionBox.y + PLAYER_HEIGHT / 2) - LOGICAL_SCREEN_HEIGHT / 2;
    camera.w = LOGICAL_SCREEN_WIDTH;
    camera.h = LOGICAL_SCREEN_HEIGHT;
    //Keeps the camera on screen even if the player is not in the centre
    if(camera.x < 0) camera.x = 0;
    if(camera.y < 0) camera.y = 0;
    if(camera.x > levelDimensions[save.level].w - camera.w) camera.x = levelDimensions[save.level].w - camera.w;
    if(camera.y > levelDimensions[save.level].h - camera.h) camera.y = levelDimensions[save.level].h - camera.h;
    parallaxOffset -= (camera.x - tempCamera.x) / 2;
}
void CPlayer::checkSpecialTileCollisions()
{
    int topLeftTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    for (int i = 0; i < 3; i++) {
        int curTile = topLeftTile + i * (levelDimensions[save.level].w / CTile::TILE_WIDTH);
        if(curTile >= 0 && curTile < tileCount && tiles[curTile]->getType() < TILE_EMPTY && checkCollision(mCollisionBox, tiles[curTile]->getBox())) tiles[curTile]->collisionEvent(curTile);
        if(curTile + 1 >= 0 && curTile + 1 < tileCount && tiles[curTile + 1]->getType() < TILE_EMPTY && checkCollision(mCollisionBox, tiles[curTile + 1]->getBox())) tiles[curTile + 1]->collisionEvent(curTile + 1);
        if(curTile + 2 >= 0 && curTile + 2 < tileCount && tiles[curTile + 2]->getType() < TILE_EMPTY && checkCollision(mCollisionBox, tiles[curTile + 2]->getBox())) tiles[curTile + 2]->collisionEvent(curTile + 2);
    }
}
void CPlayer::setForm(int form)
{
    if (gController && mForm != form) SDL_GameControllerRumble(gController, 0xFFFF / 4, 0xFFFF * 3/4, 150);
    //modifiedVelMultiplier is essentially the current direction of travel
    //if moving left, this is -1, if moving right, this is 1, and if not moving this is 0
    //this will be used to set the velocity to the new form's velocity in the right direction
    int modifiedVelMultiplier = 0;
    if (mVelX == mPlayerVel) modifiedVelMultiplier = 1;
    else if (mVelX == -mPlayerVel) modifiedVelMultiplier = -1;
    mVelX = 0;
    if (mForm == FORM_RAINBOW && form != FORM_RAINBOW) {
        rainbowTimeTimerSeconds = 0.f;
    }
    mForm = form;
    switch (form) {
        case FORM_WHITE: //100% speed 100% jump
            mTexture.setColour(0xFF, 0xFF, 0xFF);
            mPlayerVel = 300; //7.5 tiles per second
            mGravity = 1680;
            mJumpVelMax = 840; //5.25 tiles
            mJumpVelMin = 380;
            break;
        case FORM_RED: //150% speed 100% jump
            mTexture.setColour(0xFF, 0x00, 0x00);
            mPlayerVel = 450; //11.25 tiles per second
            mGravity = 1680;
            mJumpVelMax = 840; //5.25 tiles
            mJumpVelMin = 380;
            break;
        case FORM_GREEN: //100% speed 150% jump
            mTexture.setColour(0x00, 0xFF, 0x00);
            mPlayerVel = 300; //7.5 tiles per second
            mGravity = 2520;
            mJumpVelMax = 1260; //7.875 tiles
            mJumpVelMin = 570;
            break;
        case FORM_BLUE: //75% speed 75% jump
            mTexture.setColour(0x00, 0x00, 0xFF);
            mPlayerVel = 225; //5.625 tiles per second
            mGravity = 1260;
            mJumpVelMax = 630; //3.9375 tiles
            mJumpVelMin = 285;
            break;
        case FORM_RAINBOW: //150% speed 0% jump, has flight
            mHealth = save.maxHealth;
            mTexture.setColour(0xFF, 0xFF, 0xFF);
            mPlayerVel = 450;
            mGravity = 0;
            mJumpVelMax = 0;
            mJumpVelMin = 0;
            mVelY = 0;
            Mix_PlayChannel(SFX_RAINBOW, sfx[SFX_RAINBOW], 0);
            break;
    }
    mVelX = mPlayerVel * modifiedVelMultiplier;
    save.form = mForm;
}
void CPlayer::setCustomForm(int playerVel, int jumpMax, int jumpMin, int gravity)
{
    //modifiedVelMultiplier is essentially the current direction of travel
    //if moving left, this is -1, if moving right, this is 1, and if not moving this is 0
    //this will be used to set the velocity to the new form's velocity in the right direction
    int modifiedVelMultiplier = 0;
    if (mVelX == mPlayerVel || mVelX == -mPlayerVel) {
        modifiedVelMultiplier = mVelX == mPlayerVel ? 1 : -1;
        mVelX = 0;
    }
    mPlayerVel = playerVel;
    mJumpVelMax = jumpMax;
    mJumpVelMin = jumpMin;
    mGravity = gravity;
    mVelX = mPlayerVel * modifiedVelMultiplier;
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
    save.charge = charge;
}
void CPlayer::setSafePos(int safeX, int safeY)
{
    mSafePos = {safeX, safeY};
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
        //frameSpeed starts off at 5 and ends up as 1 with maximum charge
        int frameSpeed = 1 + (int)((MAX_CHARGE - mCharge) * 4 / MAX_CHARGE);
        //the next frame of animation is after frameSpeed ticks
        //11 frames in the animation
        if (!mIsInvulnerable) mFrame = (mFrame + 1) % (frameSpeed * 11);
        if (!mIsInvulnerable || rand() % 3 == 0) mTexture.render((int)mCollisionBox.x - camera.x, (int)mCollisionBox.y - camera.y, &defaultAnimFrameClips[mFrame / frameSpeed]);
    } else {
        //colour is faded depending on the time spent in rainbow form
        mTexture.setColour(0xFF - 0x7F * rainbowTimeTimerSeconds / rainbowTimeSeconds, 0xFF - 0x7F * rainbowTimeTimerSeconds / rainbowTimeSeconds, 0xFF - 0x7F * rainbowTimeTimerSeconds / rainbowTimeSeconds);
        //rainbow form has a frame speed of 3
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
    //get the index of the tile the player is in
    int curTile = (int)(mCollisionBox.y / CTile::TILE_HEIGHT) * (levelDimensions[save.level].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH);
    //checks the current tile
    if (tiles[curTile]->getType() > TILE_EMPTY //ensures the tile to check is collidable
        && checkCollision(mCollisionBox, tiles[curTile]->getBox())) return true;
    //checks the tile to the right
    if (curTile + 1 < tileCount //ensures the tile to the right of the player exists
        && tiles[curTile + 1]->getType() > TILE_EMPTY //ensures the tile to check is collidable
        && checkCollision(mCollisionBox, tiles[curTile + 1]->getBox())) return true;
    //checks the tile below
    if (curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH < tileCount //ensures the tile below the player exists
        && tiles[curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH]->getType() > TILE_EMPTY //ensures the tile to check is collidable
        && checkCollision(mCollisionBox, tiles[curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH]->getBox())) return true;
    //checks the tile to the bottom right
    if (curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH + 1 < tileCount //ensures the tile to the bottom right of the player exists
        && tiles[curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH + 1]->getType() > TILE_EMPTY //ensures the tile to check is collidable
        && checkCollision(mCollisionBox, tiles[curTile + levelDimensions[save.level].w / CTile::TILE_WIDTH + 1]->getBox())) return true;
    return false;
}
bool CPlayer::touchesGround()
{
    //check for if the player is at the bottom of the level
    if (mCollisionBox.y == levelDimensions[save.level].h - mCollisionBox.h) return true;
    SDL_Rect groundBox = {mCollisionBox.x, mCollisionBox.y + mCollisionBox.h, mCollisionBox.w, 1};
    //gets the index of the tile directly below the player
    int bottomTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) + 1) * (levelDimensions[save.level].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH);
    //checks the tile below
    if (bottomTile < tileCount //ensures the tile below the player exists
        && tiles[bottomTile]->getType() > TILE_EMPTY //ensures the tile to check is collidable
        && checkCollision(groundBox, tiles[bottomTile]->getBox())) return true;
    //checks the tile to the bottom right
    if (bottomTile + 1 < tileCount //ensures the tile to the bottom right of the player exists
        && tiles[bottomTile + 1]->getType() > TILE_EMPTY //ensures the tile to check is collidable
        && checkCollision(groundBox, tiles[bottomTile + 1]->getBox())) return true;
    return false;
}
bool CPlayer::touchesCeiling()
{
    //check for if the player is at the top of the level
    if (mCollisionBox.y == 0) return true; 
    SDL_Rect ceilingBox = {mCollisionBox.x, mCollisionBox.y - 1, mCollisionBox.w, 1};
    //gets the index of the tile directly above the player
    int topTile = ((int)(mCollisionBox.y / CTile::TILE_HEIGHT) - 1) * (levelDimensions[save.level].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH);
    //checks the tile above
    if (topTile >= 0 //ensures the tile above the player exists
        && tiles[topTile]->getType() > TILE_EMPTY //ensures the tile to check is collidable
        && checkCollision(ceilingBox, tiles[topTile]->getBox())) return true;
    //checks the tile to the top right
    if (topTile + 1 >= 0 //ensures the tile to the top right of the player exists
        && tiles[topTile + 1]->getType() > TILE_EMPTY //ensures the tile to check is collidable
        && checkCollision(ceilingBox, tiles[topTile + 1]->getBox())) return true;
    return false;
}
bool CPlayer::touchesWall()
{
    //check for if the player is on the left wall of the level
    if (mCollisionBox.x == 0) return true;
    //check for if the palyer is on the right wall of the level
    if (mCollisionBox.x == levelDimensions[save.level].w - mCollisionBox.w) return true;
    SDL_Rect leftBox = {mCollisionBox.x - 1, mCollisionBox.y, 1, mCollisionBox.h};
    SDL_Rect rightBox = {mCollisionBox.x + mCollisionBox.w, mCollisionBox.y, 1, mCollisionBox.h};
    //gets the index of the tile directly to the left
    int leftTile = (int)(mCollisionBox.y / CTile::TILE_HEIGHT) * (levelDimensions[save.level].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH) - 1;
    //gets the index of the tile directly to the right
    int rightTile = leftTile + 2;
    //checks the tile to the left
    if (leftTile >= 0 //ensures the tile to the left of the player exists
        && tiles[leftTile]->getType() > TILE_GHOST_T //ensures the tile to check is climbable
        && checkCollision(leftBox, tiles[leftTile]->getBox())) return true;
    //checks the tile to the bottom left
    if (leftTile + levelDimensions[save.level].w / CTile::TILE_WIDTH < tileCount //ensures the tile to the bottom left of the player exists
        && tiles[leftTile + levelDimensions[save.level].w / CTile::TILE_WIDTH]->getType() > TILE_GHOST_T //ensures the tile to check is climbable
        && checkCollision(leftBox, tiles[leftTile + levelDimensions[save.level].w / CTile::TILE_WIDTH]->getBox())) return true;
    //checks the tile to the right
    if (rightTile < tileCount //ensures the tile to the right of the player exists
        && tiles[rightTile]->getType() > TILE_GHOST_T //ensures the tile to check is climbable
        && checkCollision(rightBox, tiles[rightTile]->getBox())) return true;
    //checks the tile to the bottom right
    if (rightTile + levelDimensions[save.level].w / CTile::TILE_WIDTH < tileCount //ensures the tile to the bottom right of the player exists
        && tiles[rightTile + levelDimensions[save.level].w / CTile::TILE_WIDTH]->getType() > TILE_GHOST_T //ensures the tile to check is climbable
        && checkCollision(rightBox, tiles[rightTile + levelDimensions[save.level].w / CTile::TILE_WIDTH]->getBox())) return true;
    return false;
}
SDL_Point CPlayer::getNearestCollision(int xVel, int yVel, SDL_Rect oldBox)
{
    SDL_Point point = {mCollisionBox.x, mCollisionBox.y};
    int curTile = (int)(mCollisionBox.y / CTile::TILE_HEIGHT) * (levelDimensions[save.level].w / CTile::TILE_WIDTH) + (int)(mCollisionBox.x / CTile::TILE_WIDTH);
    for (int i = 0; i < 2; i++) {
        curTile += i * levelDimensions[save.level].w / CTile::TILE_WIDTH;
        for (int j = curTile; j < curTile + 2; j++) {
            if (j < 0 || j >= tileCount) continue;
            if (tiles[j]->getType() > TILE_EMPTY && checkCollision(mCollisionBox, tiles[j]->getBox())) {
                //if the player used to be in the vertical range of the tile at all but is moving to the right, the position is set back to the left of the tile
                if(xVel > 0 && oldBox.y > tiles[j]->getBox().y - oldBox.h && oldBox.y < tiles[j]->getBox().y + tiles[j]->getBox().h) point.x = tiles[j]->getBox().x - oldBox.w;
                //if the player used to be in the vertical range of the tile at all but is moving to the left, the position is set back to the right of the tile
                else if(xVel < 0 && oldBox.y > tiles[j]->getBox().y - oldBox.h && oldBox.y < tiles[j]->getBox().y + tiles[j]->getBox().h) point.x = tiles[j]->getBox().x + tiles[i]->getBox().w;
                //if the player used to be in the horizontal range of the tile at all but is moving downwards, the position is set back to the top of the tile
                if(yVel > 0 && oldBox.x > tiles[j]->getBox().x - oldBox.w && oldBox.x < tiles[j]->getBox().x + tiles[j]->getBox().w) point.y = tiles[j]->getBox().y - oldBox.h;
                //if the player used to be in the horizontal range of the tile at all but is moving upwards, the position is set back to the bottom of the tile
                else if(yVel < 0 && oldBox.x > tiles[j]->getBox().x - oldBox.w && oldBox.x < tiles[j]->getBox().x + tiles[j]->getBox().w) point.y = tiles[j]->getBox().y + tiles[i]->getBox().h;
            }
        }
    }
    return point;
}