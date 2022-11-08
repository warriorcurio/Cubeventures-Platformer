#include "LMainMenu.h"

SDL_Color gColour = {0xFF, 0xFF, 0xFF, 0xFF};
LButton button1 = {50, 50, 100, 36, "res/first.png", "Among Us", gColour, &callback};

void callback() {
    printf("hi!\n");
}

void mainMenuHandleEvent(SDL_Event* e)
{
    button1.handleEvent(e);
}

void mainMenuUpdate()
{

}

void mainMenuRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    bg.render(0, 0);
    button1.render();
    SDL_RenderPresent(gRenderer);
}