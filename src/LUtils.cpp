#include "LUtils.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_GameController* gController = NULL;
int curButton = -1;

const int LOGICAL_SCREEN_WIDTH = 1920;
const int LOGICAL_SCREEN_HEIGHT = 1080;

const int JOYSTICK_DEAD_ZONE = 8000;
Uint8 controllerRGB[3] = {0xFF, 0x00, 0x00};

int maxLevel;
int maxScore;
bool hasEverFinishedGame;

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

void menuHandleButtonSwitching(SDL_Event* e, int totalButtons)
{
    if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_TAB) curButton = (curButton + 1) % totalButtons;
    else if (e->type == SDL_JOYBUTTONUP) {
        if (e->jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP || e->jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) curButton = (curButton - 1 + totalButtons) % totalButtons;
        else if (e->jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN || e->jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) curButton = (curButton + 1) % totalButtons;
    }
}
void changeControllerRGB()
{
    if (player) {
        switch (player->getForm()) {
            case FORM_WHITE:
                SDL_GameControllerSetLED(gController, 0xFF * player->getHealth() / save.maxHealth, 0xFF * player->getHealth() / save.maxHealth, 0xFF * player->getHealth() / save.maxHealth);
                break;
            case FORM_RED:
                SDL_GameControllerSetLED(gController, 0xFF * player->getHealth() / save.maxHealth, 0x00, 0x00);
                break;
           case FORM_GREEN:
                SDL_GameControllerSetLED(gController, 0x00, 0xFF * player->getHealth() / save.maxHealth, 0x00);
                break;
            case FORM_BLUE:
                SDL_GameControllerSetLED(gController, 0x00, 0x00, 0xFF * player->getHealth() / save.maxHealth);
                break;
        }
    } else {
        SDL_GameControllerSetLED(gController, controllerRGB[0], controllerRGB[1], controllerRGB[2]);
        if(controllerRGB[2] == 0xFF) {
            controllerRGB[1] == 0 ? controllerRGB[0]++ : controllerRGB[1]--;
            if (controllerRGB[0] == 0xFF) controllerRGB[2]--;
        }
        else if(controllerRGB[1] == 0xFF) controllerRGB[0] == 0 ? controllerRGB[2]++ : controllerRGB[0]--;
        else if(controllerRGB[0] == 0xFF) controllerRGB[2] == 0 ? controllerRGB[1]++ : controllerRGB[2]--;
    }
}
void setWindowIcon(int iconNum)
{
    SDL_Surface* icons = IMG_Load("res/icons.png");
    SDL_Surface* icon = SDL_CreateRGBSurfaceWithFormat(icons->flags, 28, 28, icons->format->BitsPerPixel, SDL_PIXELFORMAT_RGBA32);
    SDL_Rect iconRect = {iconNum * 28, 0, 28, 28};
    SDL_BlitSurface(icons, &iconRect, icon, NULL);
    SDL_SetWindowIcon(gWindow, icon);
    SDL_FreeSurface(icons);
    SDL_FreeSurface(icon);
}
void savePersistent()
{
    Uint32 windowFlags = SDL_GetWindowFlags(gWindow);
    SDL_RWops* writeFile = SDL_RWFromFile("saves/persistent.bin", "wb");
    SDL_RWwrite(writeFile, &curRes, sizeof(int), 1);
    SDL_RWwrite(writeFile, &windowFlags, sizeof(Uint32), 1);
    SDL_RWwrite(writeFile, &keybinds, sizeof(keybinds), 1);
    SDL_RWwrite(writeFile, &maxLevel, sizeof(int), 1);
    SDL_RWwrite(writeFile, &maxScore, sizeof(int), 1);
    SDL_RWwrite(writeFile, &hasEverFinishedGame, sizeof(bool), 1);
    SDL_RWclose(writeFile);
}
void backCall()
{
    transition(backStack.back());
    backStack.pop_back();
}
void transition(Scene scene)
{
    curButton = -1;
    if (!boolScenes[scene]()) return;
    if (scene != SCENE_PAUSE || backStack.back() != SCENE_GAME) close();
    loadMedia = boolScenes[scene];
    handleEvent = eventScenes[scene];
    update = voidScenes[scene][0];
    render = voidScenes[scene][1];
    close = voidScenes[scene][2];
}
bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        printf("SDL couldn't init: %s\n", SDL_GetError());
        return false;
    }
    if (SDL_NumJoysticks() > 0) {
        gController = SDL_GameControllerOpen(0);
        if (gController == NULL) printf("Failed to open controller.");
        else printf("Controller Connected!\n");
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
    SDL_RWread(readFile, &maxScore, sizeof(int), 1);
    SDL_RWread(readFile, &hasEverFinishedGame, sizeof(bool), 1);
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
        printf("SDL_image couldn't initialise: %s\n", IMG_GetError());
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