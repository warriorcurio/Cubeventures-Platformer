#include "LUtils.h"

void runMainLoop();

bool quit = false;

int main(int argc, char* args[])
{
    if (!init()) {
        printf("Failed to initialise\n");
        return 1;
    }
    if (!loadMedia()) {
        printf("Failed to load media\n");
        return 2;
    }
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            handleEvent(&e);
        }
        runMainLoop();
    }
    close();
    menuBackground.free();
    menuOverlay.free();
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    SDL_GameControllerClose(gController);
    gWindow = NULL;
    gRenderer = NULL;
    gController = NULL;
    return 0;
}

void runMainLoop()
{
    if (!gController && SDL_NumJoysticks() > 0) gController = SDL_GameControllerOpen(0);
    else if (gController && SDL_NumJoysticks() == 0) gController = NULL;
    else if (gController) changeControllerRGB();
    update();
    render();
    SDL_RenderPresent(gRenderer);
}