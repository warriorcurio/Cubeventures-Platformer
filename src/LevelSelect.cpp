#include "LevelSelect.h"

SDL_Color levelSelectButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string levelSelectButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

LButton* levelSelectButtons[LEVELSELECT_BUTTON_TOTAL];

LTexture levelBG;
LTexture textLevelSelect;

#define GEN_LEVELSELECT_CALL(NUMBER)\
    void levelSelect##NUMBER##Call()\
    {\
        backStack.push_back(SCENE_LEVELSELECT);\
        save.level = (int)LEVELSELECT_BUTTON_##NUMBER;\
        save.x = 0;\
        save.y = 0;\
        transition(SCENE_DIFFICULTYSELECT);\
    }
GEN_LEVELSELECT_CALL(ONE);
GEN_LEVELSELECT_CALL(TWO);
GEN_LEVELSELECT_CALL(THREE);
GEN_LEVELSELECT_CALL(FOUR);
GEN_LEVELSELECT_CALL(FIVE);
GEN_LEVELSELECT_CALL(SIX);
GEN_LEVELSELECT_CALL(SEVEN);
GEN_LEVELSELECT_CALL(EIGHT);
GEN_LEVELSELECT_CALL(NINE);
GEN_LEVELSELECT_CALL(TEN);

bool levelSelectLoadMedia()
{
    levelBG.loadFromFile("res/saveslots.png");
    textLevelSelect.loadFromRenderedText("Level Select", levelSelectButtonTextColour, "res/04b.TTF", 40);
    levelSelectButtons[LEVELSELECT_BUTTON_BACK]  = new LButton(  10, 1020, 40, levelSelectButtonBackgroundColours, "Back", levelSelectButtonTextColour, &backCall);
    levelSelectButtons[LEVELSELECT_BUTTON_ONE]   = new LButton( 485, 335, 90, levelSelectButtonBackgroundColours,  " ", levelSelectButtonTextColour, &levelSelectONECall,   150, 150);
    levelSelectButtons[LEVELSELECT_BUTTON_ONE]->setBGFromPath("res/levelSelectONE.png");
    levelSelectButtons[LEVELSELECT_BUTTON_TWO]   = new LButton( 680, 335, 90, levelSelectButtonBackgroundColours,  " ", levelSelectButtonTextColour, &levelSelectTWOCall,   150, 150);
    levelSelectButtons[LEVELSELECT_BUTTON_TWO]->setBGFromPath("res/levelSelectTWO.png");
    levelSelectButtons[LEVELSELECT_BUTTON_THREE] = new LButton( 885, 335, 90, levelSelectButtonBackgroundColours,  "3", levelSelectButtonTextColour, &levelSelectTHREECall, 150, 150);
    levelSelectButtons[LEVELSELECT_BUTTON_FOUR]  = new LButton(1085, 335, 90, levelSelectButtonBackgroundColours,  "4", levelSelectButtonTextColour, &levelSelectFOURCall,  150, 150);
    levelSelectButtons[LEVELSELECT_BUTTON_FIVE]  = new LButton(1285, 335, 90, levelSelectButtonBackgroundColours,  "5", levelSelectButtonTextColour, &levelSelectFIVECall,  150, 150);
    levelSelectButtons[LEVELSELECT_BUTTON_SIX]   = new LButton( 485, 595, 90, levelSelectButtonBackgroundColours,  "6", levelSelectButtonTextColour, &levelSelectSIXCall,   150, 150);
    levelSelectButtons[LEVELSELECT_BUTTON_SEVEN] = new LButton( 680, 595, 90, levelSelectButtonBackgroundColours,  "7", levelSelectButtonTextColour, &levelSelectSEVENCall, 150, 150);
    levelSelectButtons[LEVELSELECT_BUTTON_EIGHT] = new LButton( 885, 595, 90, levelSelectButtonBackgroundColours,  "8", levelSelectButtonTextColour, &levelSelectEIGHTCall, 150, 150);
    levelSelectButtons[LEVELSELECT_BUTTON_NINE]  = new LButton(1085, 595, 90, levelSelectButtonBackgroundColours,  "9", levelSelectButtonTextColour, &levelSelectNINECall,  150, 150);
    levelSelectButtons[LEVELSELECT_BUTTON_TEN]   = new LButton(1285, 595, 90, levelSelectButtonBackgroundColours, "10", levelSelectButtonTextColour, &levelSelectTENCall,   150, 150);
    return true;
}
void levelSelectHandleEvent(SDL_Event* e)
{
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
    levelBG.render(0, 0);
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
    levelBG.free();
    textLevelSelect.free();
}