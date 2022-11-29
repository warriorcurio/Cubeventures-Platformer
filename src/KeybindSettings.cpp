#include "KeybindSettings.h"


SDL_Color keybindSettingsButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string keybindSettingsButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

int keybinds[KEYBINDS_TOTAL] = {SDLK_w, SDLK_a, SDLK_s, SDLK_d, SDLK_SPACE};
int curKeybind = -1;

LTexture keybindSettingsBG;

LButton* keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_TOTAL];

#define GEN_KEYBINDSETTINGS_CALL(KEYBIND)\
    void keybindSettings##KEYBIND##Call()\
    {\
        if (curKeybind == -1) curKeybind = KEYBINDS_##KEYBIND;\
        else curKeybind = -1;\
        return;\
    }

GEN_KEYBINDSETTINGS_CALL(UP);
GEN_KEYBINDSETTINGS_CALL(LEFT);
GEN_KEYBINDSETTINGS_CALL(DOWN);
GEN_KEYBINDSETTINGS_CALL(RIGHT);
GEN_KEYBINDSETTINGS_CALL(JUMP);

bool keybindSettingsLoadMedia()
{
    keybindSettingsBG.loadFromFile("res/saveslots.png");
    keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_BACK] = new LButton(10, 1020, 40, keybindSettingsButtonBackgroundColours, "Back", keybindSettingsButtonTextColour, &backCall);
    keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_UP] = new LButton(0, 0, 60, keybindSettingsButtonBackgroundColours, "UP", keybindSettingsButtonTextColour, &keybindSettingsUPCall);
    keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_LEFT] = new LButton(0, 0, 60, keybindSettingsButtonBackgroundColours, "LEFT", keybindSettingsButtonTextColour, &keybindSettingsLEFTCall);
    keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_DOWN] = new LButton(0, 0, 60, keybindSettingsButtonBackgroundColours, "DOWN", keybindSettingsButtonTextColour, &keybindSettingsDOWNCall);
    keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_RIGHT] = new LButton(0, 0, 60, keybindSettingsButtonBackgroundColours, "RIGHT", keybindSettingsButtonTextColour, &keybindSettingsRIGHTCall);
    keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_JUMP] = new LButton(0, 0, 60, keybindSettingsButtonBackgroundColours, "JUMP", keybindSettingsButtonTextColour, &keybindSettingsJUMPCall);
    for (int i = KEYBINDSETTINGS_BUTTON_UP; i < KEYBINDSETTINGS_BUTTON_TOTAL; i++) {
        keybindSettingsButtons[i]->setPos((LOGICAL_SCREEN_WIDTH - keybindSettingsButtons[i]->getW()) / 2, 355 + 75 * (i - 1));
    }
    return true;
}
void keybindSettingsHandleEvent(SDL_Event* e)
{
    for (int i = 0; i < KEYBINDSETTINGS_BUTTON_TOTAL; i++) {
        if (keybindSettingsButtons[i]) keybindSettingsButtons[i]->handleEvent(e);
    }
    if (curKeybind == -1 || e->type != SDL_KEYUP) return;
    keybinds[curKeybind] = e->key.keysym.sym;
    curKeybind = -1;
}
void keybindSettingsUpdate()
{

}
void keybindSettingsRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    keybindSettingsBG.render(0, 0);
    for (int i = 0; i < KEYBINDSETTINGS_BUTTON_TOTAL; i++) {
        if (keybindSettingsButtons[i]) keybindSettingsButtons[i]->render();
    }
}
void keybindSettingsClose()
{
    for (int i = 0; i < KEYBINDSETTINGS_BUTTON_TOTAL; i++) {
        if (keybindSettingsButtons[i]) delete keybindSettingsButtons[i];
    }
    keybindSettingsBG.free();
}