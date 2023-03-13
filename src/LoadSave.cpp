#include "LoadSave.h"
#include "CProjectile.h"

CButton* loadSaveButtons[LOADSAVE_BUTTON_TOTAL];

SDL_Rect saveHeartClips[3] = {{0, 0, 60, 60}, {0, 60, 60, 60}, {0, 120, 60, 60}};
std::string saveFileNames[3] = {"saves/save_ONE.bin", "saves/save_TWO.bin", "saves/save_THREE.bin"};
std::string saveButtonPaths[LEVEL_TOTAL] = {"ONE.png", "TWO.png", "THREE.png", "FOUR.png", "FIVE.png", "SIX.png"};
std::string difficultyNames[3] = {"Easy", "Medium", "Hard"};
Save saveSlots[3];

CTexture textLoadSave, saveHearts;
CTexture textLoadScores[3], textLoadNumDeaths[3], textLoadTimes[3], textLoadNumKeys[3], textLoadNames[3], textLoadDifficulties[3];

//creates a generator for the load save buttons
//creates a generator for the delete save buttons
#define GEN_LOADSAVE_CALL(NUMBER)\
    void loadSave##NUMBER##Call()\
    {\
        SDL_RWops* readFile = SDL_RWFromFile(saveFileNames[LOADSAVE_BUTTON_##NUMBER].c_str(), "rb");\
        SDL_RWread(readFile, &save, sizeof(Save), 1);\
        projectiles.clear();\
        projectiles.push_back(new CProjectile(0, 0, (ProjectileTypes)0, 0, 0));\
        /*the last member in the projectile vector is written into from the file*/\
        while (SDL_RWread(readFile, projectiles.back(), sizeof(CProjectile), 1) != 0) {\
            projectiles.push_back(new CProjectile(0, 0, (ProjectileTypes)0, 0, 0));\
        }\
        /*deletes the extra projectile pointer created from the loop*/\
        delete projectiles.back();\
        projectiles.pop_back();\
        SDL_RWclose(readFile);\
        Mix_HaltMusic();\
        transition(SCENE_GAME);\
    }\
    void loadSaveDel##NUMBER##Call()\
    {\
        remove("saves/save_"#NUMBER".bin");\
        loadSaveButtons[LOADSAVE_BUTTON_##NUMBER]->setClickable(false);\
        loadSaveButtons[LOADSAVE_BUTTON_DEL##NUMBER]->setPos(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT);\
    }
//generates the load and delete save buttons
GEN_LOADSAVE_CALL(ONE);
GEN_LOADSAVE_CALL(TWO);
GEN_LOADSAVE_CALL(THREE);

bool loadSaveLoadMedia()
{
    textLoadSave.loadFromRenderedText("Load Save", SDL_Color{0xFF, 0xFF, 0xFF}, 40);
    //reads each save file and puts relevant data in text textures
    for (int i = 0; i < 3; i++) {
        if (!std::filesystem::exists(saveFileNames[i].c_str())) continue;
        SDL_RWops* readFile = SDL_RWFromFile(saveFileNames[i].c_str(), "rb");
        SDL_RWread(readFile, &(saveSlots[i]), sizeof(Save), 1);
        SDL_RWclose(readFile);
        textLoadScores[i].loadFromRenderedText(std::to_string(saveSlots[i].score), SDL_Color{0xFF, 0xFF, 0xFF}, 34);
        char* timeString = (char*)calloc(5, sizeof(char));
        sprintf(timeString, "%02d:%02d", (int)(saveSlots[i].totalTime / 60), (int)saveSlots[i].totalTime % 60);
        textLoadTimes[i].loadFromRenderedText(timeString, SDL_Color{0xFF, 0xFF, 0xFF}, 34);
        textLoadNumDeaths[i].loadFromRenderedText(std::to_string(saveSlots[i].deaths), SDL_Color{0xFF, 0xFF, 0xFF}, 24);
        textLoadNumKeys[i].loadFromRenderedText(std::to_string(saveSlots[i].keys), SDL_Color{0xFF, 0xFF, 0xFF}, 24);
        textLoadDifficulties[i].loadFromRenderedText(difficultyNames[saveSlots[i].difficulty], SDL_Color{0xFF, 0xFF, 0xFF}, 50);
        textLoadNames[i].loadFromRenderedText(saveSlots[i].name, SDL_Color{0xFF, 0xFF, 0xFF}, 50);
    }
    saveHearts.loadFromFile("res/saveslots/saveHearts.png");
    loadSaveButtons[LOADSAVE_BUTTON_BACK]  = new CButton(10, 1020, 40, "Back", &backCall);
    loadSaveButtons[LOADSAVE_BUTTON_ONE] = new CButton(402, 390, 60, " ", &loadSaveONECall, 300, 300);
    loadSaveButtons[LOADSAVE_BUTTON_TWO] = new CButton(810, 390, 60, " ", &loadSaveTWOCall, 300, 300);
    loadSaveButtons[LOADSAVE_BUTTON_THREE] = new CButton(1218, 390, 60, " ", &loadSaveTHREECall, 300, 300);
    //sets the texture for a save file's level, or a default one if there is no associated save file
    for (int i = LOADSAVE_BUTTON_ONE; i <= LOADSAVE_BUTTON_THREE; i++) {
        loadSaveButtons[i]->setClickable(std::filesystem::exists(saveFileNames[i].c_str()));
        if (std::filesystem::exists(saveFileNames[i].c_str())) {
            if (saveSlots[i].finishedGame) loadSaveButtons[i]->setBGFromPath("res/saveslots/loadSaveFINAL.png");
            else loadSaveButtons[i]->setBGFromPath("res/saveslots/loadSave" + saveButtonPaths[saveSlots[i].level]);
        } else loadSaveButtons[i]->setBGFromPath("res/saveslots/loadSaveONE.png");
    }
    loadSaveButtons[LOADSAVE_BUTTON_DELONE] = new CButton(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT, 40, " ", &loadSaveDelONECall);
    loadSaveButtons[LOADSAVE_BUTTON_DELTWO] = new CButton(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT, 40, " ", &loadSaveDelTWOCall);
    loadSaveButtons[LOADSAVE_BUTTON_DELTHREE] = new CButton(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT, 40, " ", &loadSaveDelTHREECall);
    std::string deleteBackgroundColours[3] = {"#FF0000", "#7F0000", "#7F1F00"};
    for (int i = LOADSAVE_BUTTON_DELONE; i <= LOADSAVE_BUTTON_DELTHREE; i++) {
        loadSaveButtons[i]->setLabelFromPath("res/trash.png");
        loadSaveButtons[i]->setBGFromSVG(deleteBackgroundColours);
        if (std::filesystem::exists(saveFileNames[i - 3].c_str())) loadSaveButtons[i]->setPos(loadSaveButtons[i - 3]->getX() + (loadSaveButtons[i - 3]->getW() - loadSaveButtons[i]->getW()) / 2, loadSaveButtons[i - 3]->getY() + loadSaveButtons[i - 3]->getH() + 1);
        else loadSaveButtons[i]->setPos(LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT);
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

}
void loadSaveRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    menuBackground.render(0, 0);
    menuOverlay.render(300, 300);
    textLoadSave.render((LOGICAL_SCREEN_WIDTH - textLoadSave.getWidth()) / 2, 325);
    for (int i = 0; i < LOADSAVE_BUTTON_TOTAL; i++) {
        if (loadSaveButtons[i]) loadSaveButtons[i]->render();
    }
    //renders the save file's textures on the buttons accordingly
    for (int i = 0; i < 3; i++) {
        if (!std::filesystem::exists(saveFileNames[i].c_str()) || loadSaveButtons[i]->getFrame() != BUTTON_MOUSE_OVER) continue;
        textLoadScores[i].render(loadSaveButtons[i]->getX() + 160, loadSaveButtons[i]->getY() + 4);
        textLoadTimes[i].render(loadSaveButtons[i]->getX() + 296 - textLoadTimes[i].getWidth(), loadSaveButtons[i]->getY() + 43);
        textLoadNumDeaths[i].render(loadSaveButtons[i]->getX() + 160, loadSaveButtons[i]->getY() + 87);
        textLoadNumKeys[i].render(loadSaveButtons[i]->getX() + 261, loadSaveButtons[i]->getY() + 87);
        for (int j = 0; j < saveSlots[i].maxHealth; j++) {
            saveHearts.render(loadSaveButtons[i]->getX() + 60 * (saveSlots[i].difficulty + j), loadSaveButtons[i]->getY() + 120, &saveHeartClips[0]);
            if (j < saveSlots[i].curHealth && saveSlots[i].difficulty != DIFFICULTY_EASY) saveHearts.render(loadSaveButtons[i]->getX() + 60 * (saveSlots[i].difficulty + j), loadSaveButtons[i]->getY() + 120, &saveHeartClips[1]);
            if (j < saveSlots[i].curHealth && saveSlots[i].difficulty == DIFFICULTY_EASY) saveHearts.render(loadSaveButtons[i]->getX() + 60 * (saveSlots[i].difficulty + j), loadSaveButtons[i]->getY() + 120, &saveHeartClips[2]);
        }
        textLoadDifficulties[i].render((300 - textLoadDifficulties[i].getWidth()) / 2 + loadSaveButtons[i]->getX(), loadSaveButtons[i]->getY() + 183);
        textLoadNames[i].render((300 - textLoadNames[i].getWidth()) / 2 + loadSaveButtons[i]->getX(), loadSaveButtons[i]->getY() + 243);
    }
}
void loadSaveClose()
{
    for (int i = 0; i < LOADSAVE_BUTTON_TOTAL; i++) {
        if (loadSaveButtons[i]) delete loadSaveButtons[i];
    }
    textLoadSave.free();
    for (int i = 0; i < 3; i++) {
        textLoadScores[i].free();
        textLoadNumDeaths[i].free();
        textLoadNames[i].free();
        textLoadTimes[i].free();
        textLoadNumKeys[i].free();
        textLoadDifficulties[i].free();
    }
    saveHearts.free();
}