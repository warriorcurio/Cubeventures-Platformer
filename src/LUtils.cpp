#include "LUtils.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

const int LOGICAL_SCREEN_WIDTH = 1920;
const int LOGICAL_SCREEN_HEIGHT = 1080;

int maxLevel;

typedef void (*voidProcedure)();
typedef bool (*boolProcedure)();
typedef void (*handleEventProcedure)(SDL_Event*);

voidProcedure voidScenes[SCENE_TOTAL][3] = {
    {&mainMenuUpdate, &mainMenuRender, &mainMenuClose},
    {&settingsUpdate, &settingsRender, &settingsClose},
    {&levelSelectUpdate, &levelSelectRender, &levelSelectClose},
    {&createSaveUpdate, &createSaveRender, &createSaveClose},
    {&difficultySelectUpdate, &difficultySelectRender, &difficultySelectClose},
    {&loadSaveUpdate, &loadSaveRender, &loadSaveClose},
    {&nameSaveUpdate, &nameSaveRender, &nameSaveClose},
    {&gameUpdate, &gameRender, &gameClose},
    {&pauseUpdate, &pauseRender, &pauseClose},
    {&mapEditorUpdate, &mapEditorRender, &mapEditorClose},
    {&keybindSettingsUpdate, &keybindSettingsRender, &keybindSettingsClose}
};

boolProcedure boolScenes[SCENE_TOTAL] = {
    &mainMenuLoadMedia, &settingsLoadMedia, &levelSelectLoadMedia, &createSaveLoadMedia, &difficultySelectLoadMedia, &loadSaveLoadMedia, &nameSaveLoadMedia, &gameLoadMedia, &pauseLoadMedia, &mapEditorLoadMedia, &keybindSettingsLoadMedia
};

handleEventProcedure eventScenes[SCENE_TOTAL] = {
    &mainMenuHandleEvent, &settingsHandleEvent, &levelSelectHandleEvent, &createSaveHandleEvent, &difficultySelectHandleEvent, &loadSaveHandleEvent, &nameSaveHandleEvent, &gameHandleEvent, &pauseHandleEvent, &mapEditorHandleEvent, &keybindSettingsHandleEvent
};

bool (*loadMedia)();
void (*handleEvent)(SDL_Event*);
void (*update)();
void (*render)();
void (*close)();

Save save;
std::vector<Scene> backStack;

void savePersistent()
{
    Uint32 windowFlags = SDL_GetWindowFlags(gWindow);
    SDL_RWops* writeFile = SDL_RWFromFile("saves/persistent.bin", "wb");
    SDL_RWwrite(writeFile, &curRes, sizeof(int), 1);
    SDL_RWwrite(writeFile, &windowFlags, sizeof(Uint32), 1);
    SDL_RWwrite(writeFile, &keybinds, sizeof(keybinds), 1);
    SDL_RWwrite(writeFile, &maxLevel, sizeof(int), 1);
    SDL_RWclose(writeFile);
}
void backCall()
{
    transition(backStack.back());
    backStack.pop_back();
}
void transition(Scene scene)
{
    if (!boolScenes[scene]()) return;
    if (scene != SCENE_PAUSE) close();
    loadMedia = boolScenes[scene];
    handleEvent = eventScenes[scene];
    update = voidScenes[scene][0];
    render = voidScenes[scene][1];
    close = voidScenes[scene][2];
}
bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL couldn't init: %s\n", SDL_GetError());
        return false;
    }
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    resolutions[4] = {DM.w, DM.w * 9/16};
    Uint32 windowFlags;
    SDL_RWops* readFile = SDL_RWFromFile("saves/persistent.bin", "rb");
    SDL_RWread(readFile, &curRes, sizeof(int), 1);
    SDL_RWread(readFile, &windowFlags, sizeof(Uint32), 1);
    SDL_RWread(readFile, &keybinds, sizeof(keybinds), 1);
    SDL_RWread(readFile, &maxLevel, sizeof(int), 1);
    SDL_RWclose(readFile);
    gWindow = SDL_CreateWindow("Cubeventures", (DM.w - resolutions[curRes].w)/2, (DM.h - resolutions[curRes].h) / 2, resolutions[curRes].w, resolutions[curRes].h, windowFlags);
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