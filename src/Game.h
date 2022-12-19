#ifndef GAME_H
#define GAME_H
#include "LSDL.h"
#include "LTexture.h"
#include "LPlayer.h"
#include "LButton.h"
#include "LTile.h"

enum GameButtons {
    GAME_BUTTON_DEATHRETRY,
    GAME_BUTTON_DEATHQUIT,
    GAME_BUTTON_TOTAL
};

void setLevel(int level);

void gameDeathRetryCall();
void gameDeathQuitCall();

bool gameLoadMedia();
void gameHandleEvent(SDL_Event* e);
void gameUpdate();
void gameRender();
void gameClose();

extern Uint32 timeTicks;
extern const float safePositionTimeSeconds;
extern float safePositionTimeTimerSeconds;
#endif