#include "LUtils.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

const int LOGICAL_SCREEN_WIDTH = 1920;
const int LOGICAL_SCREEN_HEIGHT = 1080;

bool (*loadMedia)();
void (*handleEvent)(SDL_Event*);
void (*update)();
void (*render)();
void (*close)();

LTexture bg;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL couldn't init: %s\n", SDL_GetError());
        return false;
    }
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    gWindow = SDL_CreateWindow("test", 0, 0, DM.w, DM.h, SDL_WINDOW_SHOWN|SDL_WINDOW_BORDERLESS);
    if (gWindow == NULL) {
        printf("Window couldn't create: %s\n", SDL_GetError());
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        printf("Renderer couldn't create: %s\n", SDL_GetError());
        return false;
    }
    SDL_RenderSetLogicalSize(gRenderer, LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT);
    int imgFlags = IMG_INIT_PNG|IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image couldn't init: %s\n", IMG_GetError());
        return false;
    }
    if (TTF_Init() == -1) {
        printf("SDL_ttf couldn't initialise: %s\n", TTF_GetError());
        return false;
    }
    loadMedia = &mainMenuLoadMedia;
    handleEvent = &mainMenuHandleEvent;
    update = &mainMenuUpdate;
    render = &mainMenuRender;
    close = &mainMenuClose;
    return true;
}