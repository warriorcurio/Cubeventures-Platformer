#include "LSDL.h"
#include "LTexture.h"
#include "LButton.h"

enum MainMenuButtons {
    MAINMENU_BUTTON_EXIT,
    MAINMENU_BUTTON_SETTINGS,
    MAINMENU_BUTTON_LEVELSELECT,
    MAINMENU_BUTTON_NEWGAME,
    MAINMENU_BUTTON_LOADGAME,
    MAINMENU_BUTTON_TOTAL
};

void mainMenuNewGameCall();
void mainMenuLoadGameCall();
void mainMenuLevelSelectCall();
void mainMenuSettingsCall();
void mainMenuExitCall();

bool mainMenuLoadMedia();
void mainMenuHandleEvent(SDL_Event* e);
void mainMenuUpdate();
void mainMenuRender();
void mainMenuClose();