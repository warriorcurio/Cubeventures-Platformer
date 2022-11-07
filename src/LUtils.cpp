#include "LUtils.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

void (*update)();
void (*render)();

LTexture bg;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL couldn't init: %s\n", SDL_GetError());
        return false;
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        printf("W: Linear texture filtering not enabled\n");
    }
    gWindow = SDL_CreateWindow("test", 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window couldn't create: %s\n", SDL_GetError());
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        printf("Renderer couldn't create: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
    int imgFlags = IMG_INIT_PNG|IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image couldn't init: %s\n", IMG_GetError());
        return false;
    }
    update = &mainMenuUpdate;
    render = &mainMenuRender;
    return true;
}

bool loadMedia()
{
    bg.loadFromFile("./res/bg.png");
    return true;
}