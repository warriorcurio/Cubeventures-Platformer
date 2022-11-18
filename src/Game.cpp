#include "Game.h"

LButton* gameButtons[GAME_BUTTON_TOTAL];

bool gameLoadMedia()
{
    return true;
}
void gameHandleEvent(SDL_Event* e)
{
    for (int i = 0; i < GAME_BUTTON_TOTAL; i++) {
        if (gameButtons[i]) gameButtons[i]->handleEvent(e);
    }
}
void gameUpdate()
{
    
}
void gameRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    for (int i = 0; i < GAME_BUTTON_TOTAL; i++) {
        if (gameButtons[i]) gameButtons[i]->render();
    }
    SDL_RenderPresent(gRenderer);
}
void gameClose()
{
    for (int i = 0; i < GAME_BUTTON_TOTAL; i++) {
        if (gameButtons[i]) delete gameButtons[i];
    }
}