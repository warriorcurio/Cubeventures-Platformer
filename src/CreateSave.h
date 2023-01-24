#include "LSDL.h"
#include "CTexture.h"
#include "CButton.h"

enum CreateSaveButtons {
    CREATESAVE_BUTTON_ONE,
    CREATESAVE_BUTTON_TWO,
    CREATESAVE_BUTTON_THREE,
    CREATESAVE_BUTTON_DELONE,
    CREATESAVE_BUTTON_DELTWO,
    CREATESAVE_BUTTON_DELTHREE,
    CREATESAVE_BUTTON_BACK,
    CREATESAVE_BUTTON_TOTAL
};

void createSaveONECall();
void createSaveTWOCall();
void createSaveTHREECall();
void createSaveDelONECall();
void createSaveDelTWOCall();
void createSaveDelTHREECall();

bool createSaveLoadMedia();
void createSaveHandleEvent(SDL_Event* e);
void createSaveUpdate();
void createSaveRender();
void createSaveClose();