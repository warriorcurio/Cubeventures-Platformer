#include "MainMenu.h"

SDL_Color mainMenuButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string mainMenuButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

LButton* mainMenuButtons[MAINMENU_BUTTON_TOTAL];

LTexture background, logo;

void newGameCall()
{
    backStack.push_back(SCENE_MAINMENU);
    save.level = 1;
    transition(SCENE_DIFFICULTYSELECT);
}
void loadGameCall()
{
    backStack.push_back(SCENE_MAINMENU);
    transition(SCENE_LOADSAVE);
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
    mainMenuButtons[MAINMENU_BUTTON_NEWGAME] = new LButton(75, 500, 40, mainMenuButtonBackgroundColours, "New Game", mainMenuButtonTextColour, &newGameCall);
    mainMenuButtons[MAINMENU_BUTTON_LOADGAME] = new LButton(75, 580, 40, mainMenuButtonBackgroundColours, "Load Game", mainMenuButtonTextColour, &loadGameCall);
    mainMenuButtons[MAINMENU_BUTTON_LEVELSELECT] = new LButton(75, 660, 40, mainMenuButtonBackgroundColours, "Level Select", mainMenuButtonTextColour, &levelSelectCall);
    mainMenuButtons[MAINMENU_BUTTON_SETTINGS] = new LButton(75, 740, 40, mainMenuButtonBackgroundColours, "Settings", mainMenuButtonTextColour, &settingsCall);
    mainMenuButtons[MAINMENU_BUTTON_EXIT] = new LButton(75, 820, 40, mainMenuButtonBackgroundColours, "Exit", mainMenuButtonTextColour, &exitCall);
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
    bool savesExist = false;
    std::string saveFileNames[3] = {"saves/save_ONE.bin", "saves/save_TWO.bin", "saves/save_THREE.bin"};
    for (int i = 0; i < 3; i++) {
        if (std::filesystem::exists(saveFileNames[i].c_str())) savesExist = true;
    }
    mainMenuButtons[MAINMENU_BUTTON_LOADGAME]->setClickable(savesExist);
}
void mainMenuRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    background.render(0, 0);
    logo.render(75, 210);
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