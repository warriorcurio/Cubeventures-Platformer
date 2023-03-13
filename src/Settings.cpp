#include "Settings.h"

Resolution resolutions[5] = {{2560, 1440}, {1920, 1080}, {1600, 900}, {1280, 720}};
int curRes = 4;

CButton* settingsButtons[SETTINGS_BUTTON_TOTAL];

void settingsWindowEditCall()
{
    std::string windowEditLabel;
    if (SDL_GetWindowFlags(gWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
        SDL_SetWindowFullscreen(gWindow, 0);
        SDL_SetWindowBordered(gWindow, SDL_FALSE);
        windowEditLabel = "Borderless";
    } else if (SDL_GetWindowFlags(gWindow) & SDL_WINDOW_BORDERLESS) {
        SDL_SetWindowBordered(gWindow, SDL_TRUE);
        windowEditLabel = "Bordered";
    } else {
        SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
        windowEditLabel = "Fullscreen";
    }
    //deletes the window edit button to create a new one with the updated window state as the label
    delete settingsButtons[SETTINGS_BUTTON_WINDOWEDIT];
    settingsButtons[SETTINGS_BUTTON_WINDOWEDIT] = new CButton(0, 0, 40, windowEditLabel.c_str(), &settingsWindowEditCall);
    settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->setPos((LOGICAL_SCREEN_WIDTH - settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getH()) / 2 - (20 + settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getH()));
}
void settingsResolutionCall()
{
    int tempRes = curRes;
    curRes = (curRes + 1) % 5;
    SDL_DisplayMode DM;
    SDL_GetDesktopDisplayMode(0, &DM);
    //cycles through the resolutions until either looping back to the current resolution or finding a different resolution which is smaller than the screen size
    while (curRes != tempRes && (resolutions[curRes].w > DM.w || resolutions[curRes].w == resolutions[tempRes].w)) {
        curRes = (curRes + 1) % 5;
    }
    SDL_SetWindowSize(gWindow, resolutions[curRes].w, resolutions[curRes].h);
    SDL_SetWindowPosition(gWindow, (DM.w - resolutions[curRes].w) / 2, (DM.h - resolutions[curRes].h) / 2);
    //deletes the resolution button to create a new one with the updated resolution as the label
    char res[11];
    sprintf(res, "%dx%d", resolutions[curRes].w, resolutions[curRes].h);
    delete settingsButtons[SETTINGS_BUTTON_RESOLUTION];
    settingsButtons[SETTINGS_BUTTON_RESOLUTION] = new CButton(0, 0, 40, res, &settingsResolutionCall);
    settingsButtons[SETTINGS_BUTTON_RESOLUTION]->setPos((LOGICAL_SCREEN_WIDTH - settingsButtons[SETTINGS_BUTTON_RESOLUTION]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_RESOLUTION]->getH()) / 2);
}
void settingsKeybindsCall()
{
    backStack.push_back(SCENE_SETTINGS);
    transition(SCENE_KEYBINDSETTINGS);
}
void settingsMusicVolCall()
{
    musicVolume = (musicVolume + 5) % 105;
    Mix_VolumeMusic(musicVolume * MIX_MAX_VOLUME / 100);
    delete settingsButtons[SETTINGS_BUTTON_MUSICVOL];
    char* curLabel = (char*)calloc(12, sizeof(char));
    sprintf(curLabel, "Music - %d%%", musicVolume);
    settingsButtons[SETTINGS_BUTTON_MUSICVOL] = new CButton(0, 0, 40, curLabel, &settingsMusicVolCall);
    settingsButtons[SETTINGS_BUTTON_MUSICVOL]->setPos(1180 + (440 - settingsButtons[SETTINGS_BUTTON_MUSICVOL]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_MUSICVOL]->getH()) / 2 - 10 - settingsButtons[SETTINGS_BUTTON_MUSICVOL]->getH());
}
void settingsSFXVolCall()
{
    sfxVolume = (sfxVolume + 5) % 105;
    Mix_Volume(-1, sfxVolume * MIX_MAX_VOLUME / 100);
    delete settingsButtons[SETTINGS_BUTTON_SFXVOL];
    char* curLabel = (char*)calloc(10, sizeof(char));
    sprintf(curLabel, "SFX - %d%%", sfxVolume);
    settingsButtons[SETTINGS_BUTTON_SFXVOL] = new CButton(0, 0, 40, curLabel, &settingsSFXVolCall);
    settingsButtons[SETTINGS_BUTTON_SFXVOL]->setPos(1180 + (440 - settingsButtons[SETTINGS_BUTTON_SFXVOL]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_SFXVOL]->getH()) / 2 + 10 + settingsButtons[SETTINGS_BUTTON_SFXVOL]->getH());
}
void settingsBackCall()
{
    if (backStack.back() == SCENE_PAUSE) Mix_HaltMusic();
    backCall();
}

bool settingsLoadMedia()
{
    if (Mix_PlayingMusic() == 0) {
        bgMusic = Mix_LoadMUS("res/mus/menu.wav");
        Mix_PlayMusic(bgMusic, -1);
    }
    settingsButtons[SETTINGS_BUTTON_BACK] = new CButton(10, 1020, 40, "Back", &settingsBackCall);
    char res[11];
    sprintf(res, "%dx%d", resolutions[curRes].w, resolutions[curRes].h); //creates the label for the resolution button using the current resolution
    settingsButtons[SETTINGS_BUTTON_RESOLUTION] = new CButton(0, 0, 40, res, &settingsResolutionCall);
    //sets the position of the resolution button to the middle of the screen
    settingsButtons[SETTINGS_BUTTON_RESOLUTION]->setPos((LOGICAL_SCREEN_WIDTH - settingsButtons[SETTINGS_BUTTON_RESOLUTION]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_RESOLUTION]->getH()) / 2 + 10 + settingsButtons[SETTINGS_BUTTON_RESOLUTION]->getH());
    //sets the label of the window edit button depending on the current state of the window
    std::string windowEditLabel;
    if (SDL_GetWindowFlags(gWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) windowEditLabel = "Fullscreen";
    else if (SDL_GetWindowFlags(gWindow) & SDL_WINDOW_BORDERLESS) windowEditLabel = "Borderless";
    else windowEditLabel = "Bordered";
    settingsButtons[SETTINGS_BUTTON_WINDOWEDIT] = new CButton(0, 0, 40, windowEditLabel.c_str(), &settingsWindowEditCall);
    //sets the position of the window edit button to above the resolution button in the middle of the screen
    settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->setPos((LOGICAL_SCREEN_WIDTH - settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getH()) / 2 - 10 - settingsButtons[SETTINGS_BUTTON_WINDOWEDIT]->getH());
    settingsButtons[SETTINGS_BUTTON_KEYBINDS] = new CButton(0, 0, 40, "Keybinds", &settingsKeybindsCall);
    settingsButtons[SETTINGS_BUTTON_KEYBINDS]->setPos(300 + (440 - settingsButtons[SETTINGS_BUTTON_KEYBINDS]->getW()) / 2, 515);
    char* curLabel = (char*)calloc(12, sizeof(char));
    sprintf(curLabel, "Music - %d%%", musicVolume);
    settingsButtons[SETTINGS_BUTTON_MUSICVOL] = new CButton(0, 0, 40, curLabel, &settingsMusicVolCall);
    settingsButtons[SETTINGS_BUTTON_MUSICVOL]->setPos(1180 + (440 - settingsButtons[SETTINGS_BUTTON_MUSICVOL]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_MUSICVOL]->getH()) / 2 - 10 - settingsButtons[SETTINGS_BUTTON_MUSICVOL]->getH());
    sprintf(curLabel, "SFX - %d%%", sfxVolume);
    settingsButtons[SETTINGS_BUTTON_SFXVOL] = new CButton(0, 0, 40, curLabel, &settingsSFXVolCall);
    settingsButtons[SETTINGS_BUTTON_SFXVOL]->setPos(1180 + (440 - settingsButtons[SETTINGS_BUTTON_SFXVOL]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - settingsButtons[SETTINGS_BUTTON_SFXVOL]->getH()) / 2 + 10 + settingsButtons[SETTINGS_BUTTON_SFXVOL]->getH());
    return true;
}
void settingsHandleEvent(SDL_Event* e)
{
    if (curButton != -1) settingsButtons[curButton]->setSelected(false);
    menuHandleButtonSwitching(e, SETTINGS_BUTTON_TOTAL);
    if (curButton != -1) settingsButtons[curButton]->setSelected(true);
    for (int i = 0; i < SETTINGS_BUTTON_TOTAL; i++) {
        if (settingsButtons[i]) settingsButtons[i]->handleEvent(e);
    }
}
void settingsUpdate()
{
    settingsButtons[SETTINGS_BUTTON_RESOLUTION]->setClickable(!(SDL_GetWindowFlags(gWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP));
}
void settingsRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    menuBackground.render(0, 0);
    menuOverlay.render(300, 300);
    for (int i = 0; i < SETTINGS_BUTTON_TOTAL; i++) {
        if (settingsButtons[i]) settingsButtons[i]->render();
    }
}
void settingsClose()
{
    for (int i = 0; i < SETTINGS_BUTTON_TOTAL; i++) {
        if (settingsButtons[i]) delete settingsButtons[i];
    }
    savePersistent();
}