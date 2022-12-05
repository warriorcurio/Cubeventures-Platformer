#ifndef GAME_H
#define GAME_H
#include "LSDL.h"
#include "LTexture.h"
#include "LPlayer.h"
#include "LTile.h"

void setLevel(int level);

bool gameLoadMedia();
void gameHandleEvent(SDL_Event* e);
void gameUpdate();
void gameRender();
void gameClose();

extern Uint32 timeTicks;
#endif