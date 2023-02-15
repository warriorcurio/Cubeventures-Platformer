#include "Pause.h"
#include "Game.h"

CButton* pauseButtons[PAUSE_BUTTON_TOTAL];

void pauseResumeCall()
{
    timeTicks = SDL_GetTicks();
    close();
    backStack.pop_back();
    loadMedia = &gameLoadMedia;
    handleEvent = &gameHandleEvent;
    update = &gameUpdate;
    render = &gameRender;
    close = &gameClose;
    SDL_ShowCursor(SDL_DISABLE);
}
void pauseSaveCall()
{
    char* slotFile = (char*)calloc(20, sizeof(char));
    sprintf(slotFile, "saves/save_%s.bin", save.slot);
    SDL_RWops* writeFile = SDL_RWFromFile(slotFile, "wb");
    SDL_RWwrite(writeFile, &save, sizeof(Save), 1);
    for (int i = 0; i < (int)projectiles.size(); i++) {
        if (projectiles[i]->getType() < PROJECTILE_BUTTON_TILETOGGLE) SDL_RWwrite(writeFile, projectiles[i], sizeof(CProjectile), 1);
    }
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
    backStack.pop_back();
    transition(SCENE_MAINMENU);
    gameClose();
}

bool pauseLoadMedia()
{
    pauseButtons[PAUSE_BUTTON_RESUME]  = new CButton(0, 0, 40, "Resume", &pauseResumeCall);
    pauseButtons[PAUSE_BUTTON_RESUME]->setPos((LOGICAL_SCREEN_WIDTH - pauseButtons[PAUSE_BUTTON_RESUME]->getW()) / 2, 400);
    pauseButtons[PAUSE_BUTTON_SAVE] = new CButton(0, 0, 40, "Save", &pauseSaveCall);
    pauseButtons[PAUSE_BUTTON_SAVE]->setPos((LOGICAL_SCREEN_WIDTH - pauseButtons[PAUSE_BUTTON_SAVE]->getW()) / 2, 475);
    pauseButtons[PAUSE_BUTTON_SETTINGS] = new CButton(0, 0, 40, "Settings", &pauseSettingsCall);
    pauseButtons[PAUSE_BUTTON_SETTINGS]->setPos((LOGICAL_SCREEN_WIDTH - pauseButtons[PAUSE_BUTTON_SETTINGS]->getW()) / 2, 550);
    pauseButtons[PAUSE_BUTTON_QUIT] = new CButton(0, 0, 40, "Quit", &pauseQuitCall);
    pauseButtons[PAUSE_BUTTON_QUIT]->setPos((LOGICAL_SCREEN_WIDTH - pauseButtons[PAUSE_BUTTON_QUIT]->getW()) / 2, 625);
    return true;
}
void pauseHandleEvent(SDL_Event* e)
{
    gameHandleEvent(e);
    if (curButton != -1) pauseButtons[curButton]->setSelected(false);
    menuHandleButtonSwitching(e, PAUSE_BUTTON_TOTAL);
    if (curButton != -1) pauseButtons[curButton]->setSelected(true);
    if ((e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_ESCAPE) || (e->type == SDL_JOYBUTTONUP && e->jbutton.button == SDL_CONTROLLER_BUTTON_START)) {
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