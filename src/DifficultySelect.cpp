#include "DifficultySelect.h"

SDL_Color difficultySelectButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string difficultySelectButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

LButton* difficultySelectButtons[DIFFICULTYSELECT_BUTTON_TOTAL];

LTexture diffBG;
LTexture textdifficultySelect;

void difficultySelectGeneralCall()
{
    backStack.push_back(SCENE_DIFFICULTYSELECT);
    transition(SCENE_NAMESAVE);
}
void difficultySelectEasyCall()
{
    save.difficulty = DIFFICULTY_EASY;
    save.curHealth = DIFFICULTY_EASY_HEALTH;
    save.maxHealth = DIFFICULTY_EASY_HEALTH;
    difficultySelectGeneralCall();
}
void difficultySelectMediumCall()
{
    save.difficulty = DIFFICULTY_MEDIUM;
    save.curHealth = DIFFICULTY_MEDIUM_HEALTH;
    save.maxHealth = DIFFICULTY_MEDIUM_HEALTH;
    difficultySelectGeneralCall();
}
void difficultySelectHardCall()
{
    save.difficulty = DIFFICULTY_HARD;
    save.curHealth = DIFFICULTY_HARD_HEALTH;
    save.maxHealth = DIFFICULTY_HARD_HEALTH;
    difficultySelectGeneralCall();
}

bool difficultySelectLoadMedia()
{
    diffBG.loadFromFile("res/saveslots.png");
    textdifficultySelect.loadFromRenderedText("Difficulty Select", difficultySelectButtonTextColour, "res/04b.TTF", 40);
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_BACK]  = new LButton(10, 1020, 40, difficultySelectButtonBackgroundColours, "Back", difficultySelectButtonTextColour, &backCall);
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_EASY] = new LButton(402, 390, 60, difficultySelectButtonBackgroundColours, "Easy", difficultySelectButtonTextColour, &difficultySelectEasyCall, 300, 300);
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_MEDIUM] = new LButton(810, 390, 60, difficultySelectButtonBackgroundColours, "Medium", difficultySelectButtonTextColour, &difficultySelectMediumCall, 300, 300);
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_HARD] = new LButton(1218, 390, 60, difficultySelectButtonBackgroundColours, "Hard", difficultySelectButtonTextColour, &difficultySelectHardCall, 300, 300);
    return true;
}
void difficultySelectHandleEvent(SDL_Event* e)
{
    for (int i = 0; i < DIFFICULTYSELECT_BUTTON_TOTAL; i++) {
        if (difficultySelectButtons[i]) difficultySelectButtons[i]->handleEvent(e);
    }
}
void difficultySelectUpdate()
{

}
void difficultySelectRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    diffBG.render(0, 0);
    textdifficultySelect.render((LOGICAL_SCREEN_WIDTH - textdifficultySelect.getWidth()) / 2, 325);
    for (int i = 0; i < DIFFICULTYSELECT_BUTTON_TOTAL; i++) {
        if (difficultySelectButtons[i]) difficultySelectButtons[i]->render();
    }
}
void difficultySelectClose()
{
    for (int i = 0; i < DIFFICULTYSELECT_BUTTON_TOTAL; i++) {
        if (difficultySelectButtons[i]) delete difficultySelectButtons[i];
    }
    diffBG.free();
    textdifficultySelect.free();
}