#include "LSDL.h"
#include "LTexture.h"
#include "LButton.h"

void newGameCall();
void levelSelectCall();
void settingsCall();
void exitCall();

bool mainMenuLoadMedia();
void mainMenuHandleEvent(SDL_Event* e);
void mainMenuUpdate();
void mainMenuRender();
void mainMenuClose();