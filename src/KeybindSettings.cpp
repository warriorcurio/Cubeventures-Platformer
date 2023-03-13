#include "KeybindSettings.h"

typedef void (*call)();
int keybinds[KEYBINDS_TOTAL] = {SDLK_w, SDLK_a, SDLK_s, SDLK_d, SDLK_SPACE, SDLK_q};
std::string keybindNames[KEYBINDS_TOTAL] = {"UP", "LEFT", "DOWN", "RIGHT", "JUMP", "ABILITY"};
call keybindCalls[KEYBINDS_TOTAL] = {&keybindSettingsUPCall, &keybindSettingsLEFTCall, &keybindSettingsDOWNCall, &keybindSettingsRIGHTCall, &keybindSettingsJUMPCall, &keybindSettingsABILITYCall};
int curKeybind = -1;

CButton* keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_TOTAL];

//creates a generator for the keybind setting buttons
#define GEN_KEYBINDSETTINGS_CALL(KEYBIND)\
    void keybindSettings##KEYBIND##Call()\
    {\
        char* curLabel = (char*)calloc(20, sizeof(char));\
        sprintf(curLabel, #KEYBIND " - %s", SDL_GetKeyName(keybinds[KEYBINDS_##KEYBIND]));\
        delete keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_##KEYBIND];\
        /*if no keybinds were being set*/\
        if (curKeybind == -1) {\
            curKeybind = KEYBINDS_##KEYBIND;\
            keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_##KEYBIND] = new CButton(0, 0, 50, curLabel, &keybindSettings##KEYBIND##Call);\
            /*red button background*/\
            std::string selectedBackgroundColours[3] = {"#FF0000", "#7F0000", "#7F1F00"};\
            keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_##KEYBIND]->setBGFromSVG(selectedBackgroundColours);\
        }\
        /*if a keybind was being set*/\
        else {\
            curKeybind = -1;\
            keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_##KEYBIND] = new CButton(0, 0, 50, curLabel, &keybindSettings##KEYBIND##Call);\
        }\
        /*recreates the button with an updated label*/\
        keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_##KEYBIND]->setPos((LOGICAL_SCREEN_WIDTH - keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_##KEYBIND]->getW()) / 2, 355 + 75 * (KEYBINDSETTINGS_BUTTON_##KEYBIND));\
        /*sets all keybind buttons to activated if no keybind is being set or deactivated if a keybind is being set*/\
        for (int i = KEYBINDSETTINGS_BUTTON_UP; i < KEYBINDSETTINGS_BUTTON_BACK; i++) {\
            keybindSettingsButtons[i]->setClickable(curKeybind == -1 || i == KEYBINDSETTINGS_BUTTON_##KEYBIND);\
        }\
        return;\
    }
//generates the keybind setting buttons
GEN_KEYBINDSETTINGS_CALL(UP);
GEN_KEYBINDSETTINGS_CALL(LEFT);
GEN_KEYBINDSETTINGS_CALL(DOWN);
GEN_KEYBINDSETTINGS_CALL(RIGHT);
GEN_KEYBINDSETTINGS_CALL(JUMP);
GEN_KEYBINDSETTINGS_CALL(ABILITY);

bool keybindSettingsLoadMedia()
{
    keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_BACK] = new CButton(10, 1020, 40, "Back", &backCall);
    char* curLabel = (char*)calloc(20, sizeof(char));
    sprintf(curLabel, "UP - %s", SDL_GetKeyName(keybinds[KEYBINDS_UP]));
    keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_UP] = new CButton(0, 0, 50, curLabel, &keybindSettingsUPCall);
    sprintf(curLabel, "LEFT - %s", SDL_GetKeyName(keybinds[KEYBINDS_LEFT]));
    keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_LEFT] = new CButton(0, 0, 50, curLabel, &keybindSettingsLEFTCall);
    sprintf(curLabel, "DOWN - %s", SDL_GetKeyName(keybinds[KEYBINDS_DOWN]));
    keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_DOWN] = new CButton(0, 0, 50, curLabel, &keybindSettingsDOWNCall);
    sprintf(curLabel, "RIGHT - %s", SDL_GetKeyName(keybinds[KEYBINDS_RIGHT]));
    keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_RIGHT] = new CButton(0, 0, 50, curLabel, &keybindSettingsRIGHTCall);
    sprintf(curLabel, "JUMP - %s", SDL_GetKeyName(keybinds[KEYBINDS_JUMP]));
    keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_JUMP] = new CButton(0, 0, 50, curLabel, &keybindSettingsJUMPCall);
    sprintf(curLabel, "ABILITY - %s", SDL_GetKeyName(keybinds[KEYBINDS_ABILITY]));
    keybindSettingsButtons[KEYBINDSETTINGS_BUTTON_ABILITY] = new CButton(0, 0, 50, curLabel, &keybindSettingsABILITYCall);
    for (int i = KEYBINDSETTINGS_BUTTON_UP; i < KEYBINDSETTINGS_BUTTON_BACK; i++) {
        keybindSettingsButtons[i]->setPos((LOGICAL_SCREEN_WIDTH - keybindSettingsButtons[i]->getW()) / 2, 355 + 65 * (i));
    }
    return true;
}
void keybindSettingsHandleEvent(SDL_Event* e)
{
    if (curButton != -1) keybindSettingsButtons[curButton]->setSelected(false);
    menuHandleButtonSwitching(e, KEYBINDSETTINGS_BUTTON_TOTAL);
    if (curButton != -1) keybindSettingsButtons[curButton]->setSelected(true);
    for (int i = 0; i < KEYBINDSETTINGS_BUTTON_TOTAL; i++) {
        if (keybindSettingsButtons[i]) keybindSettingsButtons[i]->handleEvent(e);
    }
    if (curKeybind == -1 || e->type != SDL_KEYUP) return;
    keybinds[curKeybind] = e->key.keysym.sym;
    char* curLabel = (char*)calloc(20, sizeof(char));
    sprintf(curLabel, "%s - %s", keybindNames[curKeybind].c_str(), SDL_GetKeyName(keybinds[curKeybind]));
    delete keybindSettingsButtons[curKeybind];
    keybindSettingsButtons[curKeybind] = new CButton(0, 0, 50, curLabel, keybindCalls[curKeybind]);
    keybindSettingsButtons[curKeybind]->setPos((LOGICAL_SCREEN_WIDTH - keybindSettingsButtons[curKeybind]->getW()) / 2, 355 + 65 * curKeybind);
    for (int i = KEYBINDSETTINGS_BUTTON_UP; i < KEYBINDSETTINGS_BUTTON_BACK; i++) {
        keybindSettingsButtons[i]->setClickable(true);
    }
    curKeybind = -1;
}
void keybindSettingsUpdate()
{

}
void keybindSettingsRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    menuBackground.render(0, 0);
    menuOverlay.render(300, 300);
    for (int i = 0; i < KEYBINDSETTINGS_BUTTON_TOTAL; i++) {
        if (keybindSettingsButtons[i]) keybindSettingsButtons[i]->render();
    }
}
void keybindSettingsClose()
{
    for (int i = 0; i < KEYBINDSETTINGS_BUTTON_TOTAL; i++) {
        if (keybindSettingsButtons[i]) delete keybindSettingsButtons[i];
    }
    curKeybind = -1;
    savePersistent();
}