#include "LoadSave.h"

SDL_Color loadSaveButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string loadSaveButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};
std::string loadSaveButtonDelBackgroundColours[3] = {"#FF0000", "#7F0000", "#7F1F00"};

LButton* loadSaveButtons[LOADSAVE_BUTTON_TOTAL];

LTexture loadBG;
LTexture textloadSave;

#define GEN_LOADSAVE_CALL(NUMBER)\
    void loadSave##NUMBER##Call()\
    {\
        SDL_RWops* readFile = SDL_RWFromFile("saves/save_"#NUMBER".bin", "rb");\
        SDL_RWread(readFile, &save, sizeof(Save), 1);\
        SDL_RWclose(readFile);\
        transition(SCENE_GAME);\
    }\
    void loadSaveDel##NUMBER##Call()\
    {\
        remove("saves/save_"#NUMBER".bin");\
    }
GEN_LOADSAVE_CALL(ONE);
GEN_LOADSAVE_CALL(TWO);
GEN_LOADSAVE_CALL(THREE);

bool loadSaveLoadMedia()
{
    loadBG.loadFromFile("res/saveslots.png");
    textloadSave.loadFromRenderedText("Load Save", loadSaveButtonTextColour, "res/04b.TTF", 40);
    loadSaveButtons[LOADSAVE_BUTTON_BACK]  = new LButton(10, 1020, 40, loadSaveButtonBackgroundColours, "Back", loadSaveButtonTextColour, &backCall);
    loadSaveButtons[LOADSAVE_BUTTON_ONE] = new LButton(402, 390, 60, loadSaveButtonBackgroundColours, "1", loadSaveButtonTextColour, &loadSaveONECall, 300, 300);
    loadSaveButtons[LOADSAVE_BUTTON_TWO] = new LButton(810, 390, 60, loadSaveButtonBackgroundColours, "2", loadSaveButtonTextColour, &loadSaveTWOCall, 300, 300);
    loadSaveButtons[LOADSAVE_BUTTON_THREE] = new LButton(1218, 390, 60, loadSaveButtonBackgroundColours, "3", loadSaveButtonTextColour, &loadSaveTHREECall, 300, 300);
    loadSaveButtons[LOADSAVE_BUTTON_DELONE] = new LButton(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT, 40, loadSaveButtonDelBackgroundColours, " ", loadSaveButtonTextColour, &loadSaveDelONECall);
    loadSaveButtons[LOADSAVE_BUTTON_DELTWO] = new LButton(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT, 40, loadSaveButtonDelBackgroundColours, " ", loadSaveButtonTextColour, &loadSaveDelTWOCall);
    loadSaveButtons[LOADSAVE_BUTTON_DELTHREE] = new LButton(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT, 40, loadSaveButtonDelBackgroundColours, " ", loadSaveButtonTextColour, &loadSaveDelTHREECall);
    for (int i = LOADSAVE_BUTTON_DELONE; i <= LOADSAVE_BUTTON_DELTHREE; i++) {
        loadSaveButtons[i]->setLabelFromPath("res/trash.png");
    }
    return true;
}
void loadSaveHandleEvent(SDL_Event* e)
{
    if (curButton != -1) loadSaveButtons[curButton]->setSelected(false);
    menuHandleButtonSwitching(e, LOADSAVE_BUTTON_TOTAL);
    if (curButton != -1) loadSaveButtons[curButton]->setSelected(true);
    for (int i = 0; i < LOADSAVE_BUTTON_TOTAL; i++) {
        if (loadSaveButtons[i]) loadSaveButtons[i]->handleEvent(e);
    }
}
void loadSaveUpdate()
{
    std::string saveFileNames[3] = {"saves/save_ONE.bin", "saves/save_TWO.bin", "saves/save_THREE.bin"};
    for (int i = LOADSAVE_BUTTON_ONE; i <= LOADSAVE_BUTTON_THREE; i++) {
        loadSaveButtons[i]->setClickable(std::filesystem::exists(saveFileNames[i - 1].c_str()));
    }
    for (int i = LOADSAVE_BUTTON_DELONE; i <= LOADSAVE_BUTTON_DELTHREE; i++) {
        if (std::filesystem::exists(saveFileNames[i - 4].c_str())) {
            loadSaveButtons[i]->setPos(loadSaveButtons[i - 3]->getX() + (loadSaveButtons[i - 3]->getW() - loadSaveButtons[i]->getW()) / 2, loadSaveButtons[i - 3]->getY() + loadSaveButtons[i - 3]->getH() + 1);
        } else {
            loadSaveButtons[i]->setPos(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT);
        }
    }
}
void loadSaveRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    loadBG.render(0, 0);
    textloadSave.render((LOGICAL_SCREEN_WIDTH - textloadSave.getWidth()) / 2, 325);
    for (int i = 0; i < LOADSAVE_BUTTON_TOTAL; i++) {
        if (loadSaveButtons[i]) loadSaveButtons[i]->render();
    }
}
void loadSaveClose()
{
    for (int i = 0; i < LOADSAVE_BUTTON_TOTAL; i++) {
        if (loadSaveButtons[i]) delete loadSaveButtons[i];
    }
    loadBG.free();
    textloadSave.free();
}