#include "LevelSelect.h"

CButton* levelSelectButtons[LEVELSELECT_BUTTON_TOTAL];

CTexture textLevelSelect;

//creates a generator for the level select buttons
#define GEN_LEVELSELECT_CALL(NUMBER)\
    void levelSelect##NUMBER##Call()\
    {\
        backStack.push_back(SCENE_LEVELSELECT);\
        save.level = (int)LEVEL_##NUMBER;\
        save.x = 0;\
        save.y = 0;\
        transition(SCENE_DIFFICULTYSELECT);\
    }
//generates the level select buttons
GEN_LEVELSELECT_CALL(ONE);
GEN_LEVELSELECT_CALL(TWO);
GEN_LEVELSELECT_CALL(THREE);
GEN_LEVELSELECT_CALL(FOUR);
GEN_LEVELSELECT_CALL(FIVE);
GEN_LEVELSELECT_CALL(SIX);

bool levelSelectLoadMedia()
{
    textLevelSelect.loadFromRenderedText("Level Select", SDL_Color{0xFF, 0xFF, 0xFF}, 40);
    levelSelectButtons[LEVELSELECT_BUTTON_BACK]  = new CButton(  10, 1020, 40, "Back", &backCall);
    levelSelectButtons[LEVELSELECT_BUTTON_ONE]   = new CButton( 680, 335, 90,  " ", &levelSelectONECall, 150, 150);
    levelSelectButtons[LEVELSELECT_BUTTON_TWO]   = new CButton( 885, 335, 90,  " ", &levelSelectTWOCall, 150, 150);
    levelSelectButtons[LEVELSELECT_BUTTON_THREE] = new CButton( 1085, 335, 90,  " ", &levelSelectTHREECall, 150, 150);
    levelSelectButtons[LEVELSELECT_BUTTON_FOUR]  = new CButton(680, 595, 90,  " ", &levelSelectFOURCall, 150, 150);
    levelSelectButtons[LEVELSELECT_BUTTON_FIVE]  = new CButton(885, 595, 90,  " ", &levelSelectFIVECall, 150, 150);
    levelSelectButtons[LEVELSELECT_BUTTON_SIX]   = new CButton(1085, 595, 90,  " ", &levelSelectSIXCall, 150, 150);
    for (int i = LEVELSELECT_BUTTON_ONE; i < LEVELSELECT_BUTTON_BACK; i++) {
        levelSelectButtons[i]->setBGFromPath("res/levelSelect" + saveButtonPaths[i]);
    }
    for (int i = maxLevel + 1; i < LEVELSELECT_BUTTON_BACK; i++) {
        levelSelectButtons[i]->setClickable(false);
    }
    return true;
}
void levelSelectHandleEvent(SDL_Event* e)
{
    if (curButton != -1) levelSelectButtons[curButton]->setSelected(false);
    menuHandleButtonSwitching(e, LEVELSELECT_BUTTON_TOTAL);
    if (curButton != -1) levelSelectButtons[curButton]->setSelected(true);
    for (int i = 0; i < LEVELSELECT_BUTTON_TOTAL; i++) {
        if (levelSelectButtons[i]) levelSelectButtons[i]->handleEvent(e);
    }
}
void levelSelectUpdate()
{
    
}
void levelSelectRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    menuBackground.render(0, 0);
    menuOverlay.render(300, 300);
    textLevelSelect.render((LOGICAL_SCREEN_WIDTH - textLevelSelect.getWidth()) / 2, (LOGICAL_SCREEN_HEIGHT - textLevelSelect.getHeight()) / 2);
    for (int i = 0; i < LEVELSELECT_BUTTON_TOTAL; i++) {
        if (levelSelectButtons[i]) levelSelectButtons[i]->render();
    }
}
void levelSelectClose()
{
    for (int i = 0; i < LEVELSELECT_BUTTON_TOTAL; i++) {
        if (levelSelectButtons[i]) delete levelSelectButtons[i];
    }
    textLevelSelect.free();
}