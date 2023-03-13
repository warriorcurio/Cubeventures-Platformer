#include "DifficultySelect.h"

CButton* difficultySelectButtons[DIFFICULTYSELECT_BUTTON_TOTAL];

CTexture textDifficultySelect;
CTexture textDifficultyDescription;

//creates a generator for the difficulty select buttons
#define GEN_DIFFICULTYSELECT_CALL(DIFFICULTY)\
    void difficultySelect##DIFFICULTY##Call()\
    {\
        save.difficulty = DIFFICULTY_##DIFFICULTY;\
        save.curHealth = save.maxHealth = DIFFICULTY_##DIFFICULTY##_HEALTH;\
        backStack.push_back(SCENE_DIFFICULTYSELECT);\
        transition(SCENE_NAMESAVE);\
    }
//generates the difficulty select buttons
GEN_DIFFICULTYSELECT_CALL(EASY);
GEN_DIFFICULTYSELECT_CALL(MEDIUM);
GEN_DIFFICULTYSELECT_CALL(HARD);

bool difficultySelectLoadMedia()
{
    textDifficultySelect.loadFromRenderedText("Difficulty Select", SDL_Color{0xFF, 0xFF, 0xFF}, 40);
    textDifficultyDescription.loadFromRenderedText(" ", SDL_Color{0xFF, 0xFF, 0xFF}, 40);
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_BACK]  = new CButton(10, 1020, 40, "Back", &backCall);
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_EASY] = new CButton(402, 390, 60, "Easy", &difficultySelectEASYCall, 300, 300);
    // difficultySelectButtons[DIFFICULTYSELECT_BUTTON_EASY]->setLabelFromPath("res/difficultyEASY.png");
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_MEDIUM] = new CButton(810, 390, 60, "Medium", &difficultySelectMEDIUMCall, 300, 300);
    // difficultySelectButtons[DIFFICULTYSELECT_BUTTON_MEDIUM]->setLabelFromPath("res/difficultyMEDIUM.png");
    difficultySelectButtons[DIFFICULTYSELECT_BUTTON_HARD] = new CButton(1218, 390, 60, "Hard", &difficultySelectHARDCall, 300, 300);
    // difficultySelectButtons[DIFFICULTYSELECT_BUTTON_HARD]->setLabelFromPath("res/difficultyHARD.png");
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
        textDifficultySelect.loadFromRenderedText("Easy", SDL_Color{0xFF, 0xFF, 0xFF}, 40);
        textDifficultyDescription.loadFromRenderedText("5 Health, Regeneration", SDL_Color{0xFF, 0xFF, 0xFF}, 40);
    } else if (difficultySelectButtons[DIFFICULTYSELECT_BUTTON_MEDIUM]->getFrame() == BUTTON_MOUSE_OVER) {
        textDifficultySelect.loadFromRenderedText("Medium", SDL_Color{0xFF, 0xFF, 0xFF}, 40);
        textDifficultyDescription.loadFromRenderedText("3 Health", SDL_Color{0xFF, 0xFF, 0xFF}, 40);
    } else if (difficultySelectButtons[DIFFICULTYSELECT_BUTTON_HARD]->getFrame() == BUTTON_MOUSE_OVER) {
        textDifficultySelect.loadFromRenderedText("Hard", SDL_Color{0xFF, 0xFF, 0xFF}, 40);
        textDifficultyDescription.loadFromRenderedText("1 Health, Instant Death", SDL_Color{0xFF, 0xFF, 0xFF}, 40);
    } else {
        textDifficultySelect.loadFromRenderedText("Difficulty Select", SDL_Color{0xFF, 0xFF, 0xFF}, 40);
        textDifficultyDescription.loadFromRenderedText(" ", SDL_Color{0xFF, 0xFF, 0xFF}, 40);
    }
}
void difficultySelectRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    menuBackground.render(0, 0);
    menuOverlay.render(300, 300);
    textDifficultySelect.render((LOGICAL_SCREEN_WIDTH - textDifficultySelect.getWidth()) / 2, 325);
    // textDifficultyDescription.render((LOGICAL_SCREEN_WIDTH - textDifficultyDescription.getWidth()) / 2, 715);
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