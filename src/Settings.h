#include "LSDL.h"
#include "LTexture.h"
#include "LButton.h"

enum SettingsButtons {
    SETTINGS_BUTTON_BACK,
    SETTINGS_BUTTON_RESOLUTION,
    SETTINGS_BUTTON_WINDOWEDIT,
    SETTINGS_BUTTON_TOTAL
};

void windowEditCall();
void resolutionCall();

bool settingsLoadMedia();
void settingsHandleEvent(SDL_Event* e);
void settingsUpdate();
void settingsRender();
void settingsClose();