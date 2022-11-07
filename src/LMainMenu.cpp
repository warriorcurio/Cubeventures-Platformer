#include "LMainMenu.h"

void mainMenuUpdate()
{

}

void mainMenuRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    bg.render(0, 0);
    SDL_RenderPresent(gRenderer);
}