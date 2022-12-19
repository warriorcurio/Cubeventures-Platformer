#include "Pause.h"
#include "Game.h"

SDL_Color pauseButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string pauseButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

LButton* pauseButtons[PAUSE_BUTTON_TOTAL];

void pauseResumeCall()
{
    timeTicks = SDL_GetTicks();
    close();
    loadMedia = &gameLoadMedia;
    handleEvent = &gameHandleEvent;
    update = &gameUpdate;
    render = &gameRender;
    close = &gameClose;
}
void pauseSaveCall()
{
    char* slotFile = (char*)calloc(20, sizeof(char));
    sprintf(slotFile, "saves/save_%s.bin", save.slot.c_str());
    SDL_RWops* writeFile = SDL_RWFromFile(slotFile, "wb");
    SDL_RWwrite(writeFile, &save, sizeof(Save), 1);
    SDL_RWclose(writeFile);
    pauseButtons[PAUSE_BUTTON_SAVE]->setClickable(false);
}
void pauseSettingsCall()
{
    backStack.push_back(SCENE_PAUSE);
    transition(SCENE_SETTINGS);
}
void pauseQuitCall()
{
    transition(SCENE_MAINMENU);
    gameClose();
}

bool pauseLoadMedia()
{
    pauseButtons[PAUSE_BUTTON_RESUME]  = new LButton(0, 0, 40, pauseButtonBackgroundColours, "Resume", pauseButtonTextColour, &pauseResumeCall);
    pauseButtons[PAUSE_BUTTON_RESUME]->setPos((LOGICAL_SCREEN_WIDTH - pauseButtons[PAUSE_BUTTON_RESUME]->getW()) / 2, 400);
    pauseButtons[PAUSE_BUTTON_SAVE] = new LButton(0, 0, 40, pauseButtonBackgroundColours, "Save", pauseButtonTextColour, &pauseSaveCall);
    pauseButtons[PAUSE_BUTTON_SAVE]->setPos((LOGICAL_SCREEN_WIDTH - pauseButtons[PAUSE_BUTTON_SAVE]->getW()) / 2, 475);
    pauseButtons[PAUSE_BUTTON_SETTINGS] = new LButton(0, 0, 40, pauseButtonBackgroundColours, "Settings", pauseButtonTextColour, &pauseSettingsCall);
    pauseButtons[PAUSE_BUTTON_SETTINGS]->setPos((LOGICAL_SCREEN_WIDTH - pauseButtons[PAUSE_BUTTON_SETTINGS]->getW()) / 2, 550);
    pauseButtons[PAUSE_BUTTON_QUIT] = new LButton(0, 0, 40, pauseButtonBackgroundColours, "Quit", pauseButtonTextColour, &pauseQuitCall);
    pauseButtons[PAUSE_BUTTON_QUIT]->setPos((LOGICAL_SCREEN_WIDTH - pauseButtons[PAUSE_BUTTON_QUIT]->getW()) / 2, 625);
    return true;
}
void pauseHandleEvent(SDL_Event* e)
{
    gameHandleEvent(e);
    if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_ESCAPE) {
        pauseResumeCall();
    }
    for (int i = 0; i < PAUSE_BUTTON_TOTAL; i++) {
        if (pauseButtons[i]) pauseButtons[i]->handleEvent(e);
    }
}
void pauseUpdate()
{
    
}
void pauseRender()
{
    gameRender();
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xAF);
    SDL_RenderFillRect(gRenderer, NULL);
    for (int i = 0; i < PAUSE_BUTTON_TOTAL; i++) {
        if (pauseButtons[i]) pauseButtons[i]->render();
    }
}
void pauseClose()
{
    for (int i = 0; i < PAUSE_BUTTON_TOTAL; i++) {
        if (pauseButtons[i]) delete pauseButtons[i];
    }
}