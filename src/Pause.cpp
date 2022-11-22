#include "Pause.h"
#include "Game.h"

SDL_Color pauseButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string pauseButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

LButton* pauseButtons[GAME_BUTTON_TOTAL];

void pauseResumeCall()
{
    transition(SCENE_GAME);
}
void pauseSaveCall()
{
    std::string slotFile = "saves/save_" + save.slot + ".bin";
    SDL_RWops* writeFile = SDL_RWFromFile(slotFile.c_str(), "wb");
    SDL_RWwrite(writeFile, &save, sizeof(Save), 1);
    SDL_RWclose(writeFile);
}
void pauseSettingsCall()
{
    backStack.push_back(SCENE_PAUSE);
    transition(SCENE_SETTINGS);
}
void pauseQuitCall()
{
    transition(SCENE_MAINMENU);
}

bool pauseLoadMedia()
{
    pauseButtons[PAUSE_BUTTON_RESUME]  = new LButton(0, 0, 40, pauseButtonBackgroundColours, "Resume", pauseButtonTextColour, &pauseResumeCall);
    pauseButtons[PAUSE_BUTTON_SAVE] = new LButton(500, 500, 60, pauseButtonBackgroundColours, "Save", pauseButtonTextColour, &pauseSaveCall);
    pauseButtons[PAUSE_BUTTON_SETTINGS] = new LButton(1000, 1000, 60, pauseButtonBackgroundColours, "Settings", pauseButtonTextColour, &pauseSettingsCall);
    pauseButtons[PAUSE_BUTTON_QUIT] = new LButton(1500, 1500, 60, pauseButtonBackgroundColours, "Quit", pauseButtonTextColour, &pauseQuitCall);
    return true;
}
void pauseHandleEvent(SDL_Event* e)
{
    if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_ESCAPE) {
        transition(SCENE_GAME);
    }
    for (int i = 0; i < GAME_BUTTON_TOTAL; i++) {
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
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x7F);
    SDL_RenderClear(gRenderer);
    for (int i = 0; i < GAME_BUTTON_TOTAL; i++) {
        if (pauseButtons[i]) pauseButtons[i]->render();
    }
    SDL_RenderPresent(gRenderer);
}
void pauseClose()
{
    for (int i = 0; i < GAME_BUTTON_TOTAL; i++) {
        if (pauseButtons[i]) delete pauseButtons[i];
    }
}