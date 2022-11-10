#include "LMainMenu.h"

SDL_Color gLabelColour = {0xFF, 0xFF, 0xFF, 0xFF};
SDL_Color gBgColours[3] = {{0xFF, 0xFF, 0x00, 0xFF}, {0x00, 0xFF, 0xFF, 0xFF}, {0xFF, 0x00, 0x00, 0xFF}};

LButton* button1;

void callback() {
    quit = true;
    printf("%d, %d, %d", gBgColours[0].r, gBgColours[0].g, gBgColours[0].b);
}

void mainMenuHandleEvent(SDL_Event* e)
{
    button1->handleEvent(e);
}

void mainMenuUpdate()
{

}

void mainMenuRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    bg.render(0, 0);
    button1->render();
    SDL_RenderPresent(gRenderer);
}