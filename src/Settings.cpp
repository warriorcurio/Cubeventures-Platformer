#include "Settings.h"

Resolution resolutions[5] = {{2560, 1440}, {1920, 1080}, {1600, 900}, {1280, 720}};
int curRes = 4;

SDL_Color settingsButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string settingsButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

CTexture settingsBG;

CButton* settingsButtons[SETTINGS_BUTTON_TOTAL];

void settingsWindowEditCall()
{
    std::string windowEditLabel;
    if (SDL_GetWindowFlags(gWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
        SDL_SetWindowFullscreen(gWindow, 0);
        SDL_SetWindowBordered(gWindow, SDL_FALSE);
        windowEditLabel = "Borderless";
    } else if (SDL_GetWindowFlags(gWindow) & SDL_WINDOW_BORDERLESS) {
        SDL_SetWindowBordered(gWindow, SDL_TRUE);
        windowEditLabel = "Bordered";
    } else {
        SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
        windowEditLabel = "Fullscreen";
    }
    delete settingsButtons[SETTINGS_BUTTON_WINDOWEDIT];
    settingsButtons[SETTINGS_BUTTON_WINDOWEDIT] = new CButton(0, 0, 40, settingsButtonBackgroundColours, windowEditLabel.c_str(), settingsButtonTextColour, &settingsWindowEditCall);
    settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->setPos((LOGICAL_SCREEN_WIDTH - settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getH()) / 2 - (20 + settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getH()));
}
void settingsResolutionCall()
{
    int tempRes = curRes;
    curRes = (curRes + 1) % 5;
    SDL_DisplayMode DM;
    SDL_GetDesktopDisplayMode(0, &DM);
    while (curRes != tempRes && (resolutions[curRes].w > DM.w || resolutions[curRes].w == resolutions[tempRes].w)) {
        curRes = (curRes + 1) % 5;
    }
    SDL_SetWindowSize(gWindow, resolutions[curRes].w, resolutions[curRes].h);
    SDL_SetWindowPosition(gWindow, (DM.w - resolutions[curRes].w) / 2, (DM.h - resolutions[curRes].h) / 2);
    char res[11];
    sprintf(res, "%dx%d", resolutions[curRes].w, resolutions[curRes].h);
    delete settingsButtons[SETTINGS_BUTTON_RESOLUTION];
    settingsButtons[SETTINGS_BUTTON_RESOLUTION] = new CButton(0, 0, 40, settingsButtonBackgroundColours, res, settingsButtonTextColour, &settingsResolutionCall);
    settingsButtons[SETTINGS_BUTTON_RESOLUTION]->setPos((LOGICAL_SCREEN_WIDTH - settingsButtons[SETTINGS_BUTTON_RESOLUTION]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_RESOLUTION]->getH()) / 2);
}
void settingsKeybindsCall()
{
    backStack.push_back(SCENE_SETTINGS);
    transition(SCENE_KEYBINDSETTINGS);
}

bool settingsLoadMedia()
{
    settingsBG.loadFromFile("res/saveslots.png");
    settingsButtons[SETTINGS_BUTTON_BACK] = new CButton(10, 1020, 40, settingsButtonBackgroundColours, "Back", settingsButtonTextColour, &backCall);
    char res[11];
    sprintf(res, "%dx%d", resolutions[curRes].w, resolutions[curRes].h);
    settingsButtons[SETTINGS_BUTTON_RESOLUTION] = new CButton(0, 0, 40, settingsButtonBackgroundColours, res, settingsButtonTextColour, &settingsResolutionCall);
    settingsButtons[SETTINGS_BUTTON_RESOLUTION]->setPos((LOGICAL_SCREEN_WIDTH - settingsButtons[SETTINGS_BUTTON_RESOLUTION]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_RESOLUTION]->getH()) / 2);
    std::string windowEditLabel;
    if (SDL_GetWindowFlags(gWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
        windowEditLabel = "Fullscreen";
    } else if (SDL_GetWindowFlags(gWindow) & SDL_WINDOW_BORDERLESS) {
        windowEditLabel = "Borderless";
    } else {
        windowEditLabel = "Bordered";
    }
    settingsButtons[SETTINGS_BUTTON_WINDOWEDIT] = new CButton(0, 0, 40, settingsButtonBackgroundColours, windowEditLabel.c_str(), settingsButtonTextColour, &settingsWindowEditCall);
    settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->setPos((LOGICAL_SCREEN_WIDTH - settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getH()) / 2 - (20 + settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getH()));
    settingsButtons[SETTINGS_BUTTON_KEYBINDS] = new CButton(0, 0, 40, settingsButtonBackgroundColours, "Keybinds", settingsButtonTextColour, &settingsKeybindsCall);
    settingsButtons[SETTINGS_BUTTON_KEYBINDS]->setPos(300 + (440 - settingsButtons[SETTINGS_BUTTON_KEYBINDS]->getW()) / 2, 515);
    return true;
}
void settingsHandleEvent(SDL_Event* e)
{
    if (curButton != -1) settingsButtons[curButton]->setSelected(false);
    menuHandleButtonSwitching(e, SETTINGS_BUTTON_TOTAL);
    if (curButton != -1) settingsButtons[curButton]->setSelected(true);
    for (int i = 0; i < SETTINGS_BUTTON_TOTAL; i++) {
        if (settingsButtons[i]) settingsButtons[i]->handleEvent(e);
    }
}
void settingsUpdate()
{
    settingsButtons[SETTINGS_BUTTON_RESOLUTION]->setClickable(!(SDL_GetWindowFlags(gWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP));
}
void settingsRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    settingsBG.render(0, 0);
    for (int i = 0; i < SETTINGS_BUTTON_TOTAL; i++) {
        if (settingsButtons[i]) settingsButtons[i]->render();
    }
}
void settingsClose()
{
    for (int i = 0; i < SETTINGS_BUTTON_TOTAL; i++) {
        if (settingsButtons[i]) delete settingsButtons[i];
    }
    settingsBG.free();
    savePersistent();
}