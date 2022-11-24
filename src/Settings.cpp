#include "Settings.h"

Resolution resolutions[5] = {{2560, 1440}, {1920, 1080}, {1600, 900}, {1280, 720}};
int curRes = 4;

SDL_Color settingsButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string settingsButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

LButton* settingsButtons[SETTINGS_BUTTON_TOTAL];

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
    settingsButtons[SETTINGS_BUTTON_WINDOWEDIT] = new LButton(0, 0, 36, settingsButtonBackgroundColours, windowEditLabel.c_str(), settingsButtonTextColour, &settingsWindowEditCall);
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
    settingsButtons[SETTINGS_BUTTON_RESOLUTION] = new LButton(0, 0, 40, settingsButtonBackgroundColours, res, settingsButtonTextColour, &settingsResolutionCall);
    settingsButtons[SETTINGS_BUTTON_RESOLUTION]->setPos((LOGICAL_SCREEN_WIDTH - settingsButtons[SETTINGS_BUTTON_RESOLUTION]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_RESOLUTION]->getH()) / 2);
}
void settingsSaveExitCall()
{
    SDL_RWops* writeFile = SDL_RWFromFile("saves/persistent.bin", "wb");
    SDL_RWwrite(writeFile, &curRes, sizeof(int), 1);
    SDL_RWwrite(writeFile, &maxLevel, sizeof(int), 1);
    SDL_RWclose(writeFile);
    backCall();
}

bool settingsLoadMedia()
{
    settingsButtons[SETTINGS_BUTTON_BACK] = new LButton(10, 1020, 40, settingsButtonBackgroundColours, "Back", settingsButtonTextColour, &settingsSaveExitCall);
    char res[11];
    sprintf(res, "%dx%d", resolutions[curRes].w, resolutions[curRes].h);
    settingsButtons[SETTINGS_BUTTON_RESOLUTION] = new LButton(0, 0, 40, settingsButtonBackgroundColours, res, settingsButtonTextColour, &settingsResolutionCall);
    settingsButtons[SETTINGS_BUTTON_RESOLUTION]->setPos((LOGICAL_SCREEN_WIDTH - settingsButtons[SETTINGS_BUTTON_RESOLUTION]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_RESOLUTION]->getH()) / 2);
    std::string windowEditLabel;
    if (SDL_GetWindowFlags(gWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
        windowEditLabel = "Fullscreen";
    } else if (SDL_GetWindowFlags(gWindow) & SDL_WINDOW_BORDERLESS) {
        windowEditLabel = "Borderless";
    } else {
        windowEditLabel = "Bordered";
    }
    settingsButtons[SETTINGS_BUTTON_WINDOWEDIT] = new LButton(0, 0, 36, settingsButtonBackgroundColours, windowEditLabel.c_str(), settingsButtonTextColour, &settingsWindowEditCall);
    settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->setPos((LOGICAL_SCREEN_WIDTH - settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getH()) / 2 - (20 + settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getH()));
    return true;
}
void settingsHandleEvent(SDL_Event* e)
{
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
    for (int i = 0; i < SETTINGS_BUTTON_TOTAL; i++) {
        if (settingsButtons[i]) settingsButtons[i]->render();
    }
}
void settingsClose()
{
    for (int i = 0; i < SETTINGS_BUTTON_TOTAL; i++) {
        if (settingsButtons[i]) delete settingsButtons[i];
    }
}