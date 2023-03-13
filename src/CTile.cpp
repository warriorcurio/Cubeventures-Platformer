#include "CTile.h"
#include "CPlayer.h"
#include "Game.h"

CTile::CTile(int x, int y, int tileType)
{
    mCollisionBox.x = x;
    mCollisionBox.y = y;
    mCollisionBox.w = TILE_WIDTH;
    mCollisionBox.h = TILE_HEIGHT;
    mActivationTime = 0;
    mActivationCounter = 0;
    mType = tileType;
}
void CTile::collisionEvent(int tileNum)
{
    switch (mType) {
        case TILE_WHITECRYSTAL:{
            if (player->getForm() == FORM_RAINBOW) break;
            mType = TILE_WHITECRYSTAL_D;
            mActivationTime = 2.f;
            player->setForm(FORM_WHITE);
            Mix_PlayChannel(SFX_CRYSTALBREAK, sfx[SFX_CRYSTALBREAK], 0);
            break;
        }
        case TILE_REDCRYSTAL:{
            if (player->getForm() == FORM_RAINBOW) break;
            mType = TILE_REDCRYSTAL_D;
            mActivationTime = 2.f;
            player->setForm(FORM_RED);
            Mix_PlayChannel(SFX_CRYSTALBREAK, sfx[SFX_CRYSTALBREAK], 0);
            break;
        }
        case TILE_GREENCRYSTAL:{
            if (player->getForm() == FORM_RAINBOW) break;
            mType = TILE_GREENCRYSTAL_D;
            mActivationTime = 2.f;
            player->setForm(FORM_GREEN);
            Mix_PlayChannel(SFX_CRYSTALBREAK, sfx[SFX_CRYSTALBREAK], 0);
            break;
        }
        case TILE_BLUECRYSTAL:{
            if (player->getForm() == FORM_RAINBOW) break;
            mType = TILE_BLUECRYSTAL_D;
            mActivationTime = 2.f;
            player->setForm(FORM_BLUE);
            Mix_PlayChannel(SFX_CRYSTALBREAK, sfx[SFX_CRYSTALBREAK], 0);
            break;
        }
        case TILE_JUMPCRYSTAL:{
            mType = TILE_JUMPCRYSTAL_D;
            mActivationTime = 2.5f;
            player->setJumps(save.maxJumps);
            Mix_PlayChannel(SFX_CRYSTALBREAK, sfx[SFX_CRYSTALBREAK], 0);
            break;
        }
        case TILE_STEAM:{
            if (player->getForm() != FORM_RAINBOW) player->setVel(player->getVelX(), -300);
            break;
        }
        case TILE_KEY: {
            mType = TILE_EMPTY;
            player->setKeys(player->getKeys() + 1);
            for (int i = 0; i < 5; i++) {
                if (save.collectedKeys[i] == -1) {
                    save.collectedKeys[i] = tileNum;
                    std::string tempString = "Keys: " + std::to_string(i + 1) + "/5 | ";
                    for (int i = 0; i < save.maxHealth + player->getShield(); i++) {
                        if (i < save.curHealth) tempString += "❤️";
                        else if (i < save.maxHealth) tempString += "🖤";
                        else tempString += "💙";
                    }
                    activity.SetDetails(tempString.c_str());
                    core->ActivityManager().UpdateActivity(activity, NULL);
                    core->ActivityManager().UpdateActivity(activity, NULL);
                    break;
                }
            }
            Mix_PlayChannel(SFX_KEY, sfx[SFX_KEY], 0);
            if (gController) SDL_GameControllerRumble(gController, 0xFFFF / 6, 0xFFFF, 50);
            break;
        }
        case TILE_LOCK_D: {
            player->setKeys(player->getKeys() - 1);
            if (player->getKeys() < 0) {
                player->setKeys(0);
                break;
            }
            mType = TILE_EMPTY;
            for (int i = 0; i < 5; i++) {
                if (save.unlockedLocks[i] == -1) {
                    save.unlockedLocks[i] = tileNum;
                    break;
                }
            }
            Mix_PlayChannel(SFX_BREAK, sfx[SFX_BREAK], 0);
            if (gController) SDL_GameControllerRumble(gController, 0xFFFF / 6, 0xFFFF / 3, 50);
            break;
        }
        case TILE_MEDAL: {
            mType = TILE_EMPTY;
            save.score += 100;
            if (save.score > maxScore) {
                maxScore = save.score;
                savePersistent();
            }
            save.collectedMedals[save.level] = true;
            if (gController) SDL_GameControllerRumble(gController, 0xFFFF / 6, 0xFFFF / 6, 250);
            Mix_PlayChannel(SFX_MEDAL, sfx[SFX_MEDAL], 0);
            activity.SetState(("Score: " + std::to_string(save.score)).c_str());
            core->ActivityManager().UpdateActivity(activity, NULL);
            break;
        }
        case TILE_EXIT: {
            save.score += (save.difficulty + 1) * 100;
            setLevel(save.level + 1);
            Mix_PlayChannel(SFX_MEDAL, sfx[SFX_MEDAL], 0);
            if (gController) SDL_GameControllerRumble(gController, 0xFFFF / 6, 0xFFFF / 6, 1000);
            break;
        }
    }
    if (mType <= TILE_ANTIGRAV_BOTTOMRIGHT && mType >= TILE_ANTIGRAV_SOURCEMOON && player->getForm() != FORM_RAINBOW) player->setVel(player->getVelX(), 0);
    if (mType < TILE_EMPTY && mType > TILE_EXIT) {
        if (player->getInvulnerable() || player->getForm() == FORM_RAINBOW) return;
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
    }
}
void CTile::updateTiles(float timeStep)
{
    if (mActivationTime != 0) {
        mActivationCounter += timeStep;
        if (mActivationCounter < mActivationTime) return;
        if (gController) SDL_GameControllerRumble(gController, 0xFFFF * 3/4, 0xFFFF / 6, 150);
        Mix_PlayChannel(SFX_CRYSTALREACTIVATE, sfx[SFX_CRYSTALREACTIVATE], 0);
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
            case TILE_GHOST_T: if (player->getForm() == FORM_WHITE || player->getForm() == FORM_RAINBOW) mType = TILE_GHOST_T_D; break;
            case TILE_GHOST_T_D: if (player->getForm() != FORM_WHITE && player->getForm() != FORM_RAINBOW) mType = TILE_GHOST_T; break;
            case TILE_GHOST_F: if (player->getForm() != FORM_WHITE) mType = TILE_GHOST_F_D; break;
            case TILE_GHOST_F_D: if (player->getForm() == FORM_WHITE) mType = TILE_GHOST_F; break;
            case TILE_LOCK: if (player->getKeys() > 0) mType = TILE_LOCK_D; break;
            case TILE_LOCK_D: if (player->getKeys() == 0) mType = TILE_LOCK; break;
        }
    }
}
void CTile::render(SDL_Rect& camera)
{
    //tiles are only rendered if they are on screen
    if (checkCollision(camera, mCollisionBox)) tileTexture.render(mCollisionBox.x - camera.x, mCollisionBox.y - camera.y, &tileClips[mType]);
}
void CTile::setType(int type)
{
    mType = type;
}
int CTile::getType()
{
    return mType;
}
SDL_Rect CTile::getBox()
{
    return mCollisionBox;
}