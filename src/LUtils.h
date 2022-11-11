#include "LSDL.h"
#include "LMainMenu.h"
#include "LSettings.h"

enum Scene {
    SCENE_MAIN_MENU,
    SCENE_SETTINGS,
    SCENE_TOTAL
};

const int SCREEN_FPS = 60;

bool init();