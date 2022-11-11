#include "LMainMenu.h"

SDL_Color buttonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string buttonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

const int TOTAL_BUTTONS = 4;
LButton* buttons[TOTAL_BUTTONS];

LTexture background, logo;

void newGameCall()
{

}
void levelSelectCall()
{

}
void settingsCall()
{

}
void exitCall()
{
    quit = true;
}

bool mainMenuLoadMedia()
{
    if (!background.loadFromFile("res/bg.png")) return false;
    if (!logo.loadFromFile("res/logo.png")) return false;
    buttons[0] = new LButton(75, 545, 40, buttonBackgroundColours, "New Game", buttonTextColour, &newGameCall);
    buttons[1] = new LButton(75, 625, 40, buttonBackgroundColours, "Level Select", buttonTextColour, &levelSelectCall);
    buttons[2] = new LButton(75, 705, 40, buttonBackgroundColours, "Settings", buttonTextColour, &settingsCall);
    buttons[3] = new LButton(75, 785, 40, buttonBackgroundColours, "Exit", buttonTextColour, &exitCall);
    return true;
}
void mainMenuHandleEvent(SDL_Event* e)
{
    for (int i = 0; i < TOTAL_BUTTONS; i++) {
        buttons[i]->handleEvent(e);
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
    for (int i = 0; i < TOTAL_BUTTONS; i++) {
        buttons[i]->render();
    }
    SDL_RenderPresent(gRenderer);
}
void mainMenuClose()
{
    for (int i = 0; i < TOTAL_BUTTONS; i++) {
        delete buttons[i];
    }
    background.free();
    logo.free();
}