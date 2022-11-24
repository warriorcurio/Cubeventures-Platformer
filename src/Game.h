#ifndef GAME_H
#define GAME_H
#include "LSDL.h"
#include "LTexture.h"
#include "LButton.h"
#include "LPlayer.h"
#include "LTile.h"

enum GameButtons {
    GAME_BUTTON_ITEM,
    GAME_BUTTON_TOTAL
};

void nextLevel();

bool gameLoadMedia();
void gameHandleEvent(SDL_Event* e);
void gameUpdate();
void gameRender();
void gameClose();

extern Uint32 timeTicks;
#endif