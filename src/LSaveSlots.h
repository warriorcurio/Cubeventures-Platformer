#include "LSDL.h"
#include "LTexture.h"
#include "LButton.h"

enum SaveSlotsButtons {
    SAVESLOTS_BUTTON_BACK,
    SAVESLOTS_BUTTON_ONE,
    SAVESLOTS_BUTTON_TWO,
    SAVESLOTS_BUTTON_THREE,
    SAVESLOTS_BUTTON_DELONE,
    SAVESLOTS_BUTTON_DELTWO,
    SAVESLOTS_BUTTON_DELTHREE,
    SAVESLOTS_BUTTON_TOTAL
};

void saveSlotsOneCall();
void saveSlotsTwoCall();
void saveSlotsThreeCall();
void saveSlotsDelOneCall();
void saveSlotsDelTwoCall();
void saveSlotsDelThreeCall();
void saveSlotsBackCall();

bool saveSlotsLoadMedia();
void saveSlotsHandleEvent(SDL_Event* e);
void saveSlotsUpdate();
void saveSlotsRender();
void saveSlotsClose();