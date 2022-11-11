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
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                quit = true;
            }
            handleEvent(&e);
        }
        runMainLoop();
    }
    close();
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    gWindow = NULL;
    gRenderer = NULL;
    return 0;
}

void runMainLoop()
{
    update();
    render();
}