#include "LSDL.h"
#include "LTexture.h"
#include "LButton.h"

enum DifficultySelectButtons {
    DIFFICULTYSELECT_BUTTON_BACK,
    DIFFICULTYSELECT_BUTTON_EASY,
    DIFFICULTYSELECT_BUTTON_MEDIUM,
    DIFFICULTYSELECT_BUTTON_HARD,
    DIFFICULTYSELECT_BUTTON_TOTAL

};

void difficultySelectGeneralCall();
void difficultySelectEasyCall();
void difficultySelectMediumCall();
void difficultySelectHardCall();

bool difficultySelectLoadMedia();
void difficultySelectHandleEvent(SDL_Event* e);
void difficultySelectUpdate();
void difficultySelectRender();
void difficultySelectClose();