#include "DifficultySelect.h"

SDL_Color difficultySelectButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string difficultySelectButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

CButton* difficultySelectButtons[DIFFICULTYSELECT_BUTTON_TOTAL];

CTexture textDifficultySelect;
CTexture textDifficultyDescription;

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
    textDifficultySelect.loadFromRenderedText("Difficulty Select", difficultySelectButtonTextColour, "res/04b.TTF", 40);
    textDifficultyDescription.loadFromRenderedText(" ", difficultySelectButtonTextColour, "res/04b.TTF", 40);
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_BACK]  = new CButton(10, 1020, 40, difficultySelectButtonBackgroundColours, "Back", difficultySelectButtonTextColour, &backCall);
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_EASY] = new CButton(402, 390, 60, difficultySelectButtonBackgroundColours, " ", difficultySelectButtonTextColour, &difficultySelectEasyCall, 300, 300);
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_EASY]->setLabelFromPath("res/difficultyEASY.png");
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_MEDIUM] = new CButton(810, 390, 60, difficultySelectButtonBackgroundColours, " ", difficultySelectButtonTextColour, &difficultySelectMediumCall, 300, 300);
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_MEDIUM]->setLabelFromPath("res/difficultyMEDIUM.png");
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_HARD] = new CButton(1218, 390, 60, difficultySelectButtonBackgroundColours, " ", difficultySelectButtonTextColour, &difficultySelectHardCall, 300, 300);
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_HARD]->setLabelFromPath("res/difficultyHARD.png");
    return true;
}
void difficultySelectHandleEvent(SDL_Event* e)
{
    if (curButton != -1) difficultySelectButtons[curButton]->setSelected(false);
    menuHandleButtonSwitching(e, DIFFICULTYSELECT_BUTTON_TOTAL);
    if (curButton != -1) difficultySelectButtons[curButton]->setSelected(true);
    for (int i = 0; i < DIFFICULTYSELECT_BUTTON_TOTAL; i++) {
        if (difficultySelectButtons[i]) difficultySelectButtons[i]->handleEvent(e);
    }
}
void difficultySelectUpdate()
{
    if (difficultySelectButtons[DIFFICULTYSELECT_BUTTON_EASY]->getFrame() == BUTTON_MOUSE_OVER) {
        textDifficultySelect.loadFromRenderedText("Easy", difficultySelectButtonTextColour, "res/04b.TTF", 40);
        textDifficultyDescription.loadFromRenderedText("5 Health, Regeneration", difficultySelectButtonTextColour, "res/04b.TTF", 40);
    } else if (difficultySelectButtons[DIFFICULTYSELECT_BUTTON_MEDIUM]->getFrame() == BUTTON_MOUSE_OVER) {
        textDifficultySelect.loadFromRenderedText("Medium", difficultySelectButtonTextColour, "res/04b.TTF", 40);
        textDifficultyDescription.loadFromRenderedText("3 Health", difficultySelectButtonTextColour, "res/04b.TTF", 40);
    } else if (difficultySelectButtons[DIFFICULTYSELECT_BUTTON_HARD]->getFrame() == BUTTON_MOUSE_OVER) {
        textDifficultySelect.loadFromRenderedText("Hard", difficultySelectButtonTextColour, "res/04b.TTF", 40);
        textDifficultyDescription.loadFromRenderedText("1 Health, Instant Death", difficultySelectButtonTextColour, "res/04b.TTF", 40);
    } else {
        textDifficultySelect.loadFromRenderedText("Difficulty Select", difficultySelectButtonTextColour, "res/04b.TTF", 40);
        textDifficultyDescription.loadFromRenderedText(" ", difficultySelectButtonTextColour, "res/04b.TTF", 40);
    }
}
void difficultySelectRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    menuBackground.render(0, 0);
    menuOverlay.render(300, 300);
    textDifficultySelect.render((LOGICAL_SCREEN_WIDTH - textDifficultySelect.getWidth()) / 2, 325);
    textDifficultyDescription.render((LOGICAL_SCREEN_WIDTH - textDifficultyDescription.getWidth()) / 2, 715);
    for (int i = 0; i < DIFFICULTYSELECT_BUTTON_TOTAL; i++) {
        if (difficultySelectButtons[i]) difficultySelectButtons[i]->render();
    }
}
void difficultySelectClose()
{
    for (int i = 0; i < DIFFICULTYSELECT_BUTTON_TOTAL; i++) {
        if (difficultySelectButtons[i]) delete difficultySelectButtons[i];
    }
    textDifficultySelect.free();
    textDifficultyDescription.free();
}