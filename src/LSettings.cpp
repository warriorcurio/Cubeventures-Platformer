#include "LSettings.h"

SDL_Color settingsButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string settingsButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

const int SETTINGS_TOTAL_BUTTONS = 1;
LButton* settingsButtons[SETTINGS_TOTAL_BUTTONS];

void backCall()
{
    close();
    loadMedia = &mainMenuLoadMedia;
    loadMedia();
    handleEvent = &mainMenuHandleEvent;
    update = &mainMenuUpdate;
    render = &mainMenuRender;
    close = &mainMenuClose;
}

bool settingsLoadMedia()
{
    settingsButtons[0] = new LButton(10, 1020, 40, settingsButtonBackgroundColours, "Back", settingsButtonTextColour, &backCall);
    return true;
}
void settingsHandleEvent(SDL_Event* e)
{
    for (int i = 0; i < SETTINGS_TOTAL_BUTTONS; i++) {
        settingsButtons[i]->handleEvent(e);
    }
}
void settingsUpdate()
{

}
void settingsRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    for (int i = 0; i < SETTINGS_TOTAL_BUTTONS; i++) {
        settingsButtons[i]->render();
    }
    SDL_RenderPresent(gRenderer);
}
void settingsClose()
{
    for (int i = 0; i < SETTINGS_TOTAL_BUTTONS; i++) {
        delete settingsButtons[i];
    }
}