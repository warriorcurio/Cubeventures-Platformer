#include "LSDL.h"
#include "LTexture.h"
#include "LButton.h"

enum NameSaveButtons {
    NAMESAVE_BUTTON_BACK,
    NAMESAVE_BUTTON_TOTAL
};

bool nameSaveLoadMedia();
void nameSaveHandleEvent(SDL_Event* e);
void nameSaveUpdate();
void nameSaveRender();
void nameSaveClose();