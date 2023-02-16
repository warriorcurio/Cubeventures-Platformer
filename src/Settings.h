#include "LSDL.h"
#include "CTexture.h"
#include "CButton.h"

enum SettingsButtons {
    SETTINGS_BUTTON_KEYBINDS,
    SETTINGS_BUTTON_WINDOWEDIT,
    SETTINGS_BUTTON_RESOLUTION,
    SETTINGS_BUTTON_MUSICVOL,
    SETTINGS_BUTTON_SFXVOL,
    SETTINGS_BUTTON_BACK,
    SETTINGS_BUTTON_TOTAL
};

void settingsWindowEditCall();
void settingsResolutionCall();
void settingsKeybindsCall();
void settingsMusicVolCall();
void settingsSFXVolCall();
void settingsBackCall();

bool settingsLoadMedia();
void settingsHandleEvent(SDL_Event* e);
void settingsUpdate();
void settingsRender();
void settingsClose();