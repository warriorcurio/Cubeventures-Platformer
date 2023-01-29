#include "CreateSave.h"

SDL_Color createSaveButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string createSaveButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};
std::string createSaveButtonDelBackgroundColours[3] = {"#FF0000", "#7F0000", "#7F1F00"};

CButton* createSaveButtons[CREATESAVE_BUTTON_TOTAL];

CTexture textCreateSave;
CTexture textCreateScores[3], textCreateNumDeaths[3], textCreateTimes[3], textCreateNumKeys[3], textCreateNames[3], textCreateDifficulties[3];

#define GEN_CREATESAVE_CALL(NUMBER)\
    void createSave##NUMBER##Call()\
    {\
        std::strcpy(save.slot, #NUMBER);\
        save.maxJumps = save.level > 5 ? 2 : 1;\
        save.score = (200 + save.difficulty * 100) * (save.level - 1);\
        save.deaths = 0;\
        save.keys = 0;\
        save.chapterTime = 0;\
        save.totalTime = 0;\
        for (int i = 0; i < 5; i++) {\
            save.unlockedLocks[i] = -1;\
            save.collectedKeys[i] = -1;\
        }\
        for (int i = 0; i < 10; i++) {\
            save.collectedMedals[i] = false;\
        }\
        save.x = levelStartPositions[save.level - 1].x;\
        save.y = levelStartPositions[save.level - 1].y;\
        save.form = FORM_WHITE;\
        SDL_RWops* writeFile = SDL_RWFromFile("saves/save_"#NUMBER".bin", "wb");\
        SDL_RWwrite(writeFile, &save, sizeof(Save), 1);\
        SDL_RWclose(writeFile);\
        transition(SCENE_GAME);\
    }\
    void createSaveDel##NUMBER##Call()\
    {\
        remove("saves/save_"#NUMBER".bin");\
        createSaveButtons[CREATESAVE_BUTTON_##NUMBER]->setClickable(true);\
        createSaveButtons[CREATESAVE_BUTTON_##NUMBER]->setBGFromPath("res/saveslots/createSave" + saveButtonPaths[save.level - 1]);\
        createSaveButtons[CREATESAVE_BUTTON_DEL##NUMBER]->setPos(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT);\
    }
GEN_CREATESAVE_CALL(ONE);
GEN_CREATESAVE_CALL(TWO);
GEN_CREATESAVE_CALL(THREE);

bool createSaveLoadMedia()
{
    textCreateSave.loadFromRenderedText("Create Save", createSaveButtonTextColour, "res/04b.TTF", 40);
    for (int i = 0; i < 3; i++) {
        if (!std::filesystem::exists(saveFileNames[i].c_str())) continue;
        SDL_RWops* readFile = SDL_RWFromFile(saveFileNames[i].c_str(), "rb");
        SDL_RWread(readFile, &(saveSlots[i]), sizeof(Save), 1);
        SDL_RWclose(readFile);
        textCreateScores[i].loadFromRenderedText(std::to_string(saveSlots[i].score), createSaveButtonTextColour, "res/04b.TTF", 34);
        char* timeString = (char*)calloc(5, sizeof(char));
        sprintf(timeString, "%02d:%02d", (int)(saveSlots[i].totalTime / 60), (int)saveSlots[i].totalTime % 60);
        textCreateTimes[i].loadFromRenderedText(timeString, createSaveButtonTextColour, "res/04b.TTF", 34);
        textCreateNumDeaths[i].loadFromRenderedText(std::to_string(saveSlots[i].deaths), createSaveButtonTextColour, "res/04b.TTF", 24);
        textCreateNumKeys[i].loadFromRenderedText(std::to_string(saveSlots[i].keys), createSaveButtonTextColour, "res/04b.TTF", 24);
        textCreateDifficulties[i].loadFromRenderedText(difficultyNames[saveSlots[i].difficulty], createSaveButtonTextColour, "res/04b.TTF", 50);
        textCreateNames[i].loadFromRenderedText(saveSlots[i].name, createSaveButtonTextColour, "res/04b.TTF", 50);
    }
    saveHearts.loadFromFile("res/saveslots/saveHearts.png");
    createSaveButtons[CREATESAVE_BUTTON_BACK]  = new CButton(10, 1020, 40, createSaveButtonBackgroundColours, "Back", createSaveButtonTextColour, &backCall);
    createSaveButtons[CREATESAVE_BUTTON_ONE] = new CButton(402, 390, 60, createSaveButtonBackgroundColours, " ", createSaveButtonTextColour, &createSaveONECall, 300, 300);
    createSaveButtons[CREATESAVE_BUTTON_TWO] = new CButton(810, 390, 60, createSaveButtonBackgroundColours, " ", createSaveButtonTextColour, &createSaveTWOCall, 300, 300);
    createSaveButtons[CREATESAVE_BUTTON_THREE] = new CButton(1218, 390, 60, createSaveButtonBackgroundColours, " ", createSaveButtonTextColour, &createSaveTHREECall, 300, 300);
    for (int i = CREATESAVE_BUTTON_ONE; i <= CREATESAVE_BUTTON_THREE; i++) {
        createSaveButtons[i]->setClickable(!std::filesystem::exists(saveFileNames[i].c_str()));
        if (std::filesystem::exists(saveFileNames[i].c_str())) createSaveButtons[i]->setBGFromPath("res/saveslots/createSave" + saveButtonPaths[saveSlots[i].level - 1]);
        else createSaveButtons[i]->setBGFromPath("res/saveslots/createSave" + saveButtonPaths[save.level - 1]);
    }
    createSaveButtons[CREATESAVE_BUTTON_DELONE] = new CButton(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT, 40, createSaveButtonDelBackgroundColours, " ", createSaveButtonTextColour, &createSaveDelONECall);
    createSaveButtons[CREATESAVE_BUTTON_DELTWO] = new CButton(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT, 40, createSaveButtonDelBackgroundColours, " ", createSaveButtonTextColour, &createSaveDelTWOCall);
    createSaveButtons[CREATESAVE_BUTTON_DELTHREE] = new CButton(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT, 40, createSaveButtonDelBackgroundColours, " ", createSaveButtonTextColour, &createSaveDelTHREECall);
    for (int i = CREATESAVE_BUTTON_DELONE; i <= CREATESAVE_BUTTON_DELTHREE; i++) {
        createSaveButtons[i]->setLabelFromPath("res/trash.png");
        if (std::filesystem::exists(saveFileNames[i - 3].c_str())) createSaveButtons[i]->setPos(createSaveButtons[i - 3]->getX() + (createSaveButtons[i - 3]->getW() - createSaveButtons[i]->getW()) / 2, createSaveButtons[i - 3]->getY() + createSaveButtons[i - 3]->getH() + 1);
        else createSaveButtons[i]->setPos(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT);
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

}
void createSaveRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    menuBackground.render(0, 0);
    menuOverlay.render(300, 300);
    textCreateSave.render((LOGICAL_SCREEN_WIDTH - textCreateSave.getWidth()) / 2, 325);
    for (int i = 0; i < CREATESAVE_BUTTON_TOTAL; i++) {
        if (createSaveButtons[i]) createSaveButtons[i]->render();
    }
    for (int i = 0; i < 3; i++) {
        if (!std::filesystem::exists(saveFileNames[i].c_str())) continue;
        textCreateScores[i].render(createSaveButtons[i]->getX() + 160, createSaveButtons[i]->getY() + 4);
        textCreateTimes[i].render(createSaveButtons[i]->getX() + 296 - textCreateTimes[i].getWidth(), createSaveButtons[i]->getY() + 43);
        textCreateNumDeaths[i].render(createSaveButtons[i]->getX() + 160, createSaveButtons[i]->getY() + 87);
        textCreateNumKeys[i].render(createSaveButtons[i]->getX() + 261, createSaveButtons[i]->getY() + 87);
        for (int j = 0; j < saveSlots[i].maxHealth; j++) {
            saveHearts.render(createSaveButtons[i]->getX() + 60 * (saveSlots[i].difficulty + j), createSaveButtons[i]->getY() + 120, &saveHeartClips[0]);
            if (j < saveSlots[i].curHealth && saveSlots[i].difficulty != DIFFICULTY_EASY) saveHearts.render(createSaveButtons[i]->getX() + 60 * (saveSlots[i].difficulty + j), createSaveButtons[i]->getY() + 120, &saveHeartClips[1]);
            if (j < saveSlots[i].curHealth && saveSlots[i].difficulty == DIFFICULTY_EASY) saveHearts.render(createSaveButtons[i]->getX() + 60 * (saveSlots[i].difficulty + j), createSaveButtons[i]->getY() + 120, &saveHeartClips[2]);
        }
        textCreateDifficulties[i].render((300 - textCreateDifficulties[i].getWidth()) / 2 + createSaveButtons[i]->getX(), createSaveButtons[i]->getY() + 183);
        textCreateNames[i].render((300 - textCreateNames[i].getWidth()) / 2 + createSaveButtons[i]->getX(), createSaveButtons[i]->getY() + 243);
    }
}
void createSaveClose()
{
    for (int i = 0; i < CREATESAVE_BUTTON_TOTAL; i++) {
        if (createSaveButtons[i]) delete createSaveButtons[i];
    }
    textCreateSave.free();
    for (int i = 0; i < 3; i++) {
        textCreateScores[i].free();
        textCreateNumDeaths[i].free();
        textCreateNames[i].free();
        textCreateTimes[i].free();
        textCreateNumKeys[i].free();
        textCreateDifficulties[i].free();
    }
    saveHearts.free();
}