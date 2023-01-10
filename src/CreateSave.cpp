#include "CreateSave.h"

SDL_Color createSaveButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string createSaveButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};
std::string createSaveButtonDelBackgroundColours[3] = {"#FF0000", "#7F0000", "#7F1F00"};

LButton* createSaveButtons[CREATESAVE_BUTTON_TOTAL];

LTexture saveBG;
LTexture textCreateSave;

#define GEN_CREATESAVE_CALL(NUMBER)\
    void createSave##NUMBER##Call()\
    {\
        save.slot = #NUMBER;\
        save.maxJumps = save.level > 5 ? 2 : 1;\
        save.keys = 0;\
        for (int i = 0; i < 5; i++) {\
            save.unlockedLocks[i] = 0;\
            save.collectedKeys[i] = 0;\
        }\
        for (int i = 0; i < 10; i++) {\
            save.collectedMedals[i] = false;\
        }\
        save.x = 0;\
        save.y = 1020;\
        save.form = FORM_WHITE;\
        SDL_RWops* writeFile = SDL_RWFromFile("saves/save_"#NUMBER".bin", "wb");\
        SDL_RWwrite(writeFile, &save, sizeof(Save), 1);\
        SDL_RWclose(writeFile);\
        transition(SCENE_GAME);\
    }\
    void createSaveDel##NUMBER##Call()\
    {\
        remove("saves/save_"#NUMBER".bin");\
    }
GEN_CREATESAVE_CALL(ONE);
GEN_CREATESAVE_CALL(TWO);
GEN_CREATESAVE_CALL(THREE);

bool createSaveLoadMedia()
{
    saveBG.loadFromFile("res/saveslots.png");
    textCreateSave.loadFromRenderedText("Create Save", createSaveButtonTextColour, "res/04b.TTF", 40);
    createSaveButtons[CREATESAVE_BUTTON_BACK]  = new LButton(10, 1020, 40, createSaveButtonBackgroundColours, "Back", createSaveButtonTextColour, &backCall);
    createSaveButtons[CREATESAVE_BUTTON_ONE] = new LButton(402, 390, 60, createSaveButtonBackgroundColours, "1", createSaveButtonTextColour, &createSaveONECall, 300, 300);
    createSaveButtons[CREATESAVE_BUTTON_TWO] = new LButton(810, 390, 60, createSaveButtonBackgroundColours, "2", createSaveButtonTextColour, &createSaveTWOCall, 300, 300);
    createSaveButtons[CREATESAVE_BUTTON_THREE] = new LButton(1218, 390, 60, createSaveButtonBackgroundColours, "3", createSaveButtonTextColour, &createSaveTHREECall, 300, 300);
    createSaveButtons[CREATESAVE_BUTTON_DELONE] = new LButton(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT, 40, createSaveButtonDelBackgroundColours, " ", createSaveButtonTextColour, &createSaveDelONECall);
    createSaveButtons[CREATESAVE_BUTTON_DELTWO] = new LButton(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT, 40, createSaveButtonDelBackgroundColours, " ", createSaveButtonTextColour, &createSaveDelTWOCall);
    createSaveButtons[CREATESAVE_BUTTON_DELTHREE] = new LButton(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT, 40, createSaveButtonDelBackgroundColours, " ", createSaveButtonTextColour, &createSaveDelTHREECall);
    for (int i = CREATESAVE_BUTTON_DELONE; i <= CREATESAVE_BUTTON_DELTHREE; i++) {
        createSaveButtons[i]->setLabelFromPath("res/trash.png");
    }
    return true;
}
void createSaveHandleEvent(SDL_Event* e)
{
    if (curButton != -1) createSaveButtons[curButton]->setSelected(false);
    menuHandleButtonSwitching(e, CREATESAVE_BUTTON_TOTAL);
    if (curButton != -1) createSaveButtons[curButton]->setSelected(true);
    for (int i = 0; i < CREATESAVE_BUTTON_TOTAL; i++) {
        if (createSaveButtons[i]) createSaveButtons[i]->handleEvent(e);
    }
}
void createSaveUpdate()
{
    std::string saveFileNames[3] = {"saves/save_ONE.bin", "saves/save_TWO.bin", "saves/save_THREE.bin"};
    for (int i = CREATESAVE_BUTTON_ONE; i <= CREATESAVE_BUTTON_THREE; i++) {
        createSaveButtons[i]->setClickable(!std::filesystem::exists(saveFileNames[i - 1].c_str()));
    }
    for (int i = CREATESAVE_BUTTON_DELONE; i <= CREATESAVE_BUTTON_DELTHREE; i++) {
        if (std::filesystem::exists(saveFileNames[i - 4].c_str())) {
            createSaveButtons[i]->setPos(createSaveButtons[i - 3]->getX() + (createSaveButtons[i - 3]->getW() - createSaveButtons[i]->getW()) / 2, createSaveButtons[i - 3]->getY() + createSaveButtons[i - 3]->getH() + 1);
        } else {
            createSaveButtons[i]->setPos(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT);
        }
    }
}
void createSaveRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    saveBG.render(0, 0);
    textCreateSave.render((LOGICAL_SCREEN_WIDTH - textCreateSave.getWidth()) / 2, 325);
    for (int i = 0; i < CREATESAVE_BUTTON_TOTAL; i++) {
        if (createSaveButtons[i]) createSaveButtons[i]->render();
    }
}
void createSaveClose()
{
    for (int i = 0; i < CREATESAVE_BUTTON_TOTAL; i++) {
        if (createSaveButtons[i]) delete createSaveButtons[i];
    }
    saveBG.free();
    textCreateSave.free();
}