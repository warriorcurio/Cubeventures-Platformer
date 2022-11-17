#include "LSDL.h"
#include "LTexture.h"
#include "LButton.h"

enum LoadSaveButtons {
    LOADSAVE_BUTTON_BACK,
    LOADSAVE_BUTTON_ONE,
    LOADSAVE_BUTTON_TWO,
    LOADSAVE_BUTTON_THREE,
    LOADSAVE_BUTTON_DELONE,
    LOADSAVE_BUTTON_DELTWO,
    LOADSAVE_BUTTON_DELTHREE,
    LOADSAVE_BUTTON_TOTAL

};

void loadSaveONECall();
void loadSaveTWOCall();
void loadSaveTHREECall();
void loadSaveDelONECall();
void loadSaveDelTWOCall();
void loadSaveDelTHREECall();

bool loadSaveLoadMedia();
void loadSaveHandleEvent(SDL_Event* e);
void loadSaveUpdate();
void loadSaveRender();
void loadSaveClose();