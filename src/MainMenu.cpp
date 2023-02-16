#include "MainMenu.h"

CButton* mainMenuButtons[MAINMENU_BUTTON_TOTAL];

CTexture menuBackground, menuOverlay, logo, star;

void mainMenuNewGameCall()
{
    backStack.push_back(SCENE_MAINMENU);
    save.level = LEVEL_ONE;
    transition(SCENE_DIFFICULTYSELECT);
}
void mainMenuLoadGameCall()
{
    backStack.push_back(SCENE_MAINMENU);
    transition(SCENE_LOADSAVE);
}
void mainMenuLevelSelectCall()
{
    backStack.push_back(SCENE_MAINMENU);
    transition(SCENE_LEVELSELECT);
}
void mainMenuSettingsCall()
{
    backStack.push_back(SCENE_MAINMENU);
    transition(SCENE_SETTINGS);
}
void mainMenuExitCall()
{
    quit = true;
}

bool mainMenuLoadMedia()
{
    setWindowIcon(0);
    if (Mix_PlayingMusic() == 0) {
        bgMusic = Mix_LoadMUS("res/mus/menu.wav");
        Mix_PlayMusic(bgMusic, -1);
    }
    menuBackground.loadFromFile(bgNames[maxLevel]);
    menuOverlay.loadFromFile("res/menuOverlay.png");
    logo.loadFromFile("res/logo.png");
    star.loadFromFile("res/star.png");
    mainMenuButtons[MAINMENU_BUTTON_NEWGAME] = new CButton(75, 500, 40, "New Game", &mainMenuNewGameCall);
    mainMenuButtons[MAINMENU_BUTTON_LOADGAME] = new CButton(75, 580, 40, "Load Game", &mainMenuLoadGameCall);
    mainMenuButtons[MAINMENU_BUTTON_LEVELSELECT] = new CButton(75, 660, 40, "Level Select", &mainMenuLevelSelectCall);
    mainMenuButtons[MAINMENU_BUTTON_SETTINGS] = new CButton(75, 740, 40, "Settings", &mainMenuSettingsCall);
    mainMenuButtons[MAINMENU_BUTTON_EXIT] = new CButton(75, 820, 40, "Exit", &mainMenuExitCall);
    return true;
}
void mainMenuHandleEvent(SDL_Event* e)
{
    if (curButton != -1) mainMenuButtons[curButton]->setSelected(false);
    menuHandleButtonSwitching(e, MAINMENU_BUTTON_TOTAL);
    if (curButton != -1) mainMenuButtons[curButton]->setSelected(true);
    for (int i = 0; i < MAINMENU_BUTTON_TOTAL; i++) {
        if (mainMenuButtons[i]) mainMenuButtons[i]->handleEvent(e);
    }
    if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_SLASH && SDL_GetModState() & KMOD_CTRL) {
        save.level = LEVEL_ONE;
        transition(SCENE_MAPEDITOR);
    } else if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_d && SDL_GetModState() & (KMOD_CTRL | KMOD_SHIFT)) {
        curRes = 4;
        Uint32 windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS;
        keybinds[KEYBINDS_UP] = SDLK_w;
        keybinds[KEYBINDS_LEFT] = SDLK_a;
        keybinds[KEYBINDS_DOWN] = SDLK_s;
        keybinds[KEYBINDS_RIGHT] = SDLK_d;
        keybinds[KEYBINDS_JUMP] = SDLK_SPACE;
        keybinds[KEYBINDS_ABILITY] = SDLK_q;
        maxLevel = LEVEL_ONE;
        maxScore = 0;
        hasEverFinishedGame = false;
        musicVolume = 80;
        sfxVolume = 80;
        SDL_RWops* writeFile = SDL_RWFromFile("saves/persistent.bin", "wb");
        SDL_RWwrite(writeFile, &curRes, sizeof(int), 1);
        SDL_RWwrite(writeFile, &windowFlags, sizeof(Uint32), 1);
        SDL_RWwrite(writeFile, &keybinds, sizeof(keybinds), 1);
        SDL_RWwrite(writeFile, &maxLevel, sizeof(int), 1);
        SDL_RWwrite(writeFile, &maxScore, sizeof(int), 1);
        SDL_RWwrite(writeFile, &hasEverFinishedGame, sizeof(bool), 1);
        SDL_RWwrite(writeFile, &musicVolume, sizeof(int), 1);
        SDL_RWwrite(writeFile, &sfxVolume, sizeof(int), 1);
        SDL_RWclose(writeFile);
        save.~Save();
        remove("saves/save_ONE.bin");
        remove("saves/save_TWO.bin");
        remove("saves/save_THREE.bin");
        menuBackground.loadFromFile(bgNames[LEVEL_ONE]);
    }
}
void mainMenuUpdate()
{
    bool savesExist = false;
    std::string saveFileNames[3] = {"saves/save_ONE.bin", "saves/save_TWO.bin", "saves/save_THREE.bin"};
    for (int i = 0; i < 3; i++) {
        if (std::filesystem::exists(saveFileNames[i].c_str())) savesExist = true;
    }
    mainMenuButtons[MAINMENU_BUTTON_LOADGAME]->setClickable(savesExist);
}
void mainMenuRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    menuBackground.render(0, 0);
    logo.render(75, 210);
    for (int i = LEVEL_TWO; i < maxLevel; i++) {
        star.render(i * 80 - 72, 1008);
    }
    if (hasEverFinishedGame) star.render(328, 1008);
    if (maxScore == 500 * LEVEL_TOTAL) star.render(408, 1008);
    for (int i = 0; i < MAINMENU_BUTTON_TOTAL; i++) {
        if (mainMenuButtons[i]) mainMenuButtons[i]->render();
    }
}
void mainMenuClose()
{
    for (int i = 0; i < MAINMENU_BUTTON_TOTAL; i++) {
        if (mainMenuButtons[i]) delete mainMenuButtons[i];
    }
    logo.free();
}