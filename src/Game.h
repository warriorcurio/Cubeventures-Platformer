#include "LSDL.h"
#include "LTexture.h"
#include "LButton.h"
#include "LPlayer.h"

enum GameButtons {
    GAME_BUTTON_ITEM,
    GAME_BUTTON_TOTAL
};

bool gameLoadMedia();
void gameHandleEvent(SDL_Event* e);
void gameUpdate();
void gameRender();
void gameClose();