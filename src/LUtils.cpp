#include "LUtils.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

const int LOGICAL_SCREEN_WIDTH = 1920;
const int LOGICAL_SCREEN_HEIGHT = 1080;

typedef void (*voidProcedure)();
typedef bool (*boolProcedure)();
typedef void (*handleEventProcedure)(SDL_Event*);

voidProcedure voidScenes[SCENE_TOTAL * 3] = {
    &mainMenuUpdate, &mainMenuRender, &mainMenuClose,
    &settingsUpdate, &settingsRender, &settingsClose,
    &levelSelectUpdate, &levelSelectRender, &levelSelectClose,
    &createSaveUpdate, &createSaveRender, &createSaveClose,
    &difficultySelectUpdate, &difficultySelectRender, &difficultySelectClose,
    &loadSaveUpdate, &loadSaveRender, &loadSaveClose,
    &nameSaveUpdate, &nameSaveRender, &nameSaveClose
};

boolProcedure boolScenes[SCENE_TOTAL] = {
    &mainMenuLoadMedia, &settingsLoadMedia, &levelSelectLoadMedia, &createSaveLoadMedia, &difficultySelectLoadMedia, &loadSaveLoadMedia, &nameSaveLoadMedia
};

handleEventProcedure eventScenes[SCENE_TOTAL] = {
    &mainMenuHandleEvent, &settingsHandleEvent, &levelSelectHandleEvent, &createSaveHandleEvent, &difficultySelectHandleEvent, &loadSaveHandleEvent, &nameSaveHandleEvent
};

bool (*loadMedia)();
void (*handleEvent)(SDL_Event*);
void (*update)();
void (*render)();
void (*close)();

Save save;
std::vector<Scene> backStack;

void backCall()
{
    transition(backStack.back());
    backStack.pop_back();
}
void transition(Scene scene)
{
    close();
    loadMedia = boolScenes[scene];
    loadMedia();
    handleEvent = eventScenes[scene];
    update = voidScenes[scene * 3];
    render = voidScenes[1 + scene * 3];
    close = voidScenes[2 + scene * 3];
}
bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL couldn't init: %s\n", SDL_GetError());
        return false;
    }
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    gWindow = SDL_CreateWindow("Cubeventures", 0, (DM.h - (DM.w * 9/16)) / 2, DM.w, DM.w * 9/16, SDL_WINDOW_SHOWN|SDL_WINDOW_BORDERLESS);
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