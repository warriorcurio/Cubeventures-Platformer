#include "LSaveSlots.h"

SDL_Color saveSlotsButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string saveSlotsButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

LButton* saveSlotsButtons[SAVESLOTS_BUTTON_TOTAL];

LTexture saveBG;

bool saveSlotsLoadMedia()
{
    saveBG.loadFromFile("res/saveslots.png");
    saveSlotsButtons[SAVESLOTS_BUTTON_BACK]  = new LButton(10, 1020, 40, saveSlotsButtonBackgroundColours, "Back", saveSlotsButtonTextColour, &backCall);
    return true;
}
void saveSlotsHandleEvent(SDL_Event* e)
{
    for (int i = 0; i < SAVESLOTS_BUTTON_TOTAL; i++) {
        if (saveSlotsButtons[i]) saveSlotsButtons[i]->handleEvent(e);
    }
}
void saveSlotsUpdate()
{

}
void saveSlotsRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    saveBG.render(0, 0);
    for (int i = 0; i < SAVESLOTS_BUTTON_TOTAL; i++) {
        if (saveSlotsButtons[i]) saveSlotsButtons[i]->render();
    }
    SDL_RenderPresent(gRenderer);
}
void saveSlotsClose()
{
    for (int i = 0; i < SAVESLOTS_BUTTON_TOTAL; i++) {
        if (saveSlotsButtons[i]) delete saveSlotsButtons[i];
    }
}