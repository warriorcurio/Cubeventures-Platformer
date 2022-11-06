#include "LUtils.h"

void runMainLoop();

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
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                quit = true;
            }
        }
        runMainLoop();
    }
    return 0;
}

void runMainLoop()
{
    update();
    render();
}