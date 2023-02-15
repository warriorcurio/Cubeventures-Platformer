#include "LSDL.h"
#include "CTexture.h"
#include "CButton.h"

enum KeybindSettingsButtons {
    KEYBINDSETTINGS_BUTTON_UP,
    KEYBINDSETTINGS_BUTTON_LEFT,
    KEYBINDSETTINGS_BUTTON_DOWN,
    KEYBINDSETTINGS_BUTTON_RIGHT,
    KEYBINDSETTINGS_BUTTON_JUMP,
    KEYBINDSETTINGS_BUTTON_ABILITY,
    KEYBINDSETTINGS_BUTTON_BACK,
    KEYBINDSETTINGS_BUTTON_TOTAL
};

void keybindSettingsUPCall();
void keybindSettingsLEFTCall();
void keybindSettingsDOWNCall();
void keybindSettingsRIGHTCall();
void keybindSettingsJUMPCall();
void keybindSettingsABILITYCall();

bool keybindSettingsLoadMedia();
void keybindSettingsHandleEvent(SDL_Event* e);
void keybindSettingsUpdate();
void keybindSettingsRender();
void keybindSettingsClose();