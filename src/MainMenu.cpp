#include "MainMenu.h"

SDL_Color mainMenuButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string mainMenuButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

LButton* mainMenuButtons[MAINMENU_BUTTON_TOTAL];

LTexture background, logo;

void newGameCall()
{
    backStack.push_back(SCENE_MAINMENU);
    save.level = 1;
    transition(SCENE_CREATESAVE);
}
void levelSelectCall()
{
    backStack.push_back(SCENE_MAINMENU);
    transition(SCENE_LEVELSELECT);
}
void settingsCall()
{
    backStack.push_back(SCENE_MAINMENU);
    transition(SCENE_SETTINGS);
}
void exitCall()
{
    quit = true;
}

bool mainMenuLoadMedia()
{
    if (!background.loadFromFile("res/bg.png")) return false;
    if (!logo.loadFromFile("res/logo.png")) return false;
    mainMenuButtons[MAINMENU_BUTTON_NEWGAME] = new LButton(75, 545, 40, mainMenuButtonBackgroundColours, "New Game", mainMenuButtonTextColour, &newGameCall);
    mainMenuButtons[MAINMENU_BUTTON_LEVELSELECT] = new LButton(75, 625, 40, mainMenuButtonBackgroundColours, "Level Select", mainMenuButtonTextColour, &levelSelectCall);
    mainMenuButtons[MAINMENU_BUTTON_SETTINGS] = new LButton(75, 705, 40, mainMenuButtonBackgroundColours, "Settings", mainMenuButtonTextColour, &settingsCall);
    mainMenuButtons[MAINMENU_BUTTON_EXIT] = new LButton(75, 785, 40, mainMenuButtonBackgroundColours, "Exit", mainMenuButtonTextColour, &exitCall);
    return true;
}
void mainMenuHandleEvent(SDL_Event* e)
{
    for (int i = 0; i < MAINMENU_BUTTON_TOTAL; i++) {
        if (mainMenuButtons[i]) mainMenuButtons[i]->handleEvent(e);
    }
}
void mainMenuUpdate()
{
    
}
void mainMenuRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    background.render(0, 0);
    logo.render(75, 230);
    for (int i = 0; i < MAINMENU_BUTTON_TOTAL; i++) {
        if (mainMenuButtons[i]) mainMenuButtons[i]->render();
    }
    SDL_RenderPresent(gRenderer);
}
void mainMenuClose()
{
    for (int i = 0; i < MAINMENU_BUTTON_TOTAL; i++) {
        if (mainMenuButtons[i]) delete mainMenuButtons[i];
    }
    background.free();
    logo.free();
}