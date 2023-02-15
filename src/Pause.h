#include "LSDL.h"
#include "CTexture.h"
#include "CButton.h"

enum PauseButtons {
    PAUSE_BUTTON_RESUME,
    PAUSE_BUTTON_SAVE,
    PAUSE_BUTTON_SETTINGS,
    PAUSE_BUTTON_QUIT,
    PAUSE_BUTTON_TOTAL
};

void pauseResumeCall();
void pauseSaveCall();
void pauseSettingsCall();
void pauseQuitCall();

bool pauseLoadMedia();
void pauseHandleEvent(SDL_Event* e);
void pauseUpdate();
void pauseRender();
void pauseClose();