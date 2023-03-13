#include "LSDL.h"
#include "CTexture.h"
#include "CButton.h"

enum DifficultySelectButtons {
    DIFFICULTYSELECT_BUTTON_EASY,
    DIFFICULTYSELECT_BUTTON_MEDIUM,
    DIFFICULTYSELECT_BUTTON_HARD,
    DIFFICULTYSELECT_BUTTON_BACK,
    DIFFICULTYSELECT_BUTTON_TOTAL

};

void difficultySelectEASYCall();
void difficultySelectMEDIUMCall();
void difficultySelectHARDCall();

bool difficultySelectLoadMedia();
void difficultySelectHandleEvent(SDL_Event* e);
void difficultySelectUpdate();
void difficultySelectRender();
void difficultySelectClose();