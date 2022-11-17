#include "LSDL.h"
#include "LTexture.h"
#include "LButton.h"

enum LevelSelectButtons {
    LEVELSELECT_BUTTON_BACK,
    LEVELSELECT_BUTTON_ONE,
    LEVELSELECT_BUTTON_TWO,
    LEVELSELECT_BUTTON_THREE,
    LEVELSELECT_BUTTON_FOUR,
    LEVELSELECT_BUTTON_FIVE,
    LEVELSELECT_BUTTON_SIX,
    LEVELSELECT_BUTTON_SEVEN,
    LEVELSELECT_BUTTON_EIGHT,
    LEVELSELECT_BUTTON_NINE,
    LEVELSELECT_BUTTON_TEN,
    LEVELSELECT_BUTTON_TOTAL
};

void levelSelectONECall();
void levelSelectTWOCall();
void levelSelectTHREECall();
void levelSelectFOURCall();
void levelSelectFIVECall();
void levelSelectSIXCall();
void levelSelectSEVENCall();
void levelSelectEIGHTCall();
void levelSelectNINECall();
void levelSelectTENCall();

bool levelSelectLoadMedia();
void levelSelectHandleEvent(SDL_Event* e);
void levelSelectUpdate();
void levelSelectRender();
void levelSelectClose();