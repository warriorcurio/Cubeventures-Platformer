#include "LLevelSelect.h"

SDL_Color levelSelectButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string levelSelectButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

LButton* levelSelectButtons[LEVELSELECT_BUTTON_TOTAL];

void levelSelectBackCall()
{
    transition(SCENE_MAINMENU);
}

bool levelSelectLoadMedia()
{
    levelSelectButtons[LEVELSELECT_BUTTON_BACK] = new LButton(10, 1020, 40, levelSelectButtonBackgroundColours, "Back", levelSelectButtonTextColour, &levelSelectBackCall);
    levelSelectButtons[LEVELSELECT_BUTTON_ONE] = new LButton(0, 0, 50, levelSelectButtonBackgroundColours, "1", levelSelectButtonTextColour, &levelSelectBackCall);
    levelSelectButtons[LEVELSELECT_BUTTON_TWO] = new LButton(0, 0, 50, levelSelectButtonBackgroundColours, "1", levelSelectButtonTextColour, &levelSelectBackCall);
    levelSelectButtons[LEVELSELECT_BUTTON_THREE] = new LButton(0, 0, 50, levelSelectButtonBackgroundColours, "1", levelSelectButtonTextColour, &levelSelectBackCall);
    levelSelectButtons[LEVELSELECT_BUTTON_FOUR] = new LButton(0, 0, 50, levelSelectButtonBackgroundColours, "1", levelSelectButtonTextColour, &levelSelectBackCall);
    levelSelectButtons[LEVELSELECT_BUTTON_FIVE] = new LButton(0, 0, 50, levelSelectButtonBackgroundColours, "1", levelSelectButtonTextColour, &levelSelectBackCall);
    levelSelectButtons[LEVELSELECT_BUTTON_SIX] = new LButton(0, 0, 50, levelSelectButtonBackgroundColours, "1", levelSelectButtonTextColour, &levelSelectBackCall);
    levelSelectButtons[LEVELSELECT_BUTTON_SEVEN] = new LButton(0, 0, 50, levelSelectButtonBackgroundColours, "1", levelSelectButtonTextColour, &levelSelectBackCall);
    levelSelectButtons[LEVELSELECT_BUTTON_EIGHT] = new LButton(0, 0, 50, levelSelectButtonBackgroundColours, "1", levelSelectButtonTextColour, &levelSelectBackCall);
    levelSelectButtons[LEVELSELECT_BUTTON_NINE] = new LButton(0, 0, 50, levelSelectButtonBackgroundColours, "1", levelSelectButtonTextColour, &levelSelectBackCall);
    levelSelectButtons[LEVELSELECT_BUTTON_TEN] = new LButton(0, 0, 50, levelSelectButtonBackgroundColours, "1", levelSelectButtonTextColour, &levelSelectBackCall);
    return true;
}
void levelSelectHandleEvent(SDL_Event* e)
{
    for (int i = 0; i < LEVELSELECT_BUTTON_TOTAL; i++) {
        if (levelSelectButtons[i]) levelSelectButtons[i]->handleEvent(e);
    }
}
void levelSelectUpdate()
{

}
void levelSelectRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    for (int i = 0; i < LEVELSELECT_BUTTON_TOTAL; i++) {
        if (levelSelectButtons[i]) levelSelectButtons[i]->render();
    }
    SDL_RenderPresent(gRenderer);
}
void levelSelectClose()
{
    for (int i = 0; i < LEVELSELECT_BUTTON_TOTAL; i++) {
        if (levelSelectButtons[i]) delete levelSelectButtons[i];
    }
}