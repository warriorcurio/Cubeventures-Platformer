#ifndef LSDL_H
#define LSDL_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>

enum Scene {
    SCENE_MAINMENU,
    SCENE_SETTINGS,
    SCENE_LEVELSELECT,
    SCENE_CREATESAVE,
    SCENE_DIFFICULTYSELECT,
    SCENE_LOADSAVE,
    SCENE_NAMESAVE,
    SCENE_GAME,
    SCENE_PAUSE,
    SCENE_MAPEDITOR,
    SCENE_KEYBINDSETTINGS,
    SCENE_TOTAL
};

enum Levels {
    LEVEL_ONE,
    LEVEL_TWO,
    LEVEL_THREE,
    LEVEL_FOUR,
    LEVEL_FIVE,
    LEVEL_SIX,
    LEVEL_SEVEN,
    LEVEL_EIGHT,
    LEVEL_NINE,
    LEVEL_TEN,
    LEVEL_TOTAL
};

enum Tiles {
    TILE_WHITECRYSTAL,
    TILE_WHITECRYSTAL_D,
    TILE_REDCRYSTAL,
    TILE_REDCRYSTAL_D,
    TILE_GREENCRYSTAL,
    TILE_GREENCRYSTAL_D,
    TILE_BLUECRYSTAL,
    TILE_BLUECRYSTAL_D,
    TILE_JUMPCRYSTAL,
    TILE_JUMPCRYSTAL_D,
    TILE_GHOST_D,
    TILE_KEY,
    TILE_LOCK_D,
    TILE_EVENT,
    TILE_EXIT,
    TILE_SPIKE,
    TILE_EMPTY,
    TILE_GHOST,
    TILE_LOCK,
    TILE_GRASS_TOPLEFT, TILE_GRASS_TOPMIDDLE, TILE_GRASS_TOPRIGHT, TILE_GRASS_MIDDLELEFT, TILE_GRASS_MIDDLE, TILE_GRASS_MIDDLERIGHT, TILE_GRASS_BOTTOMLEFT, TILE_GRASS_BOTTOMMIDDLE, TILE_GRASS_BOTTOMRIGHT,
    TILE_GRASS_INNER_BOTTOMRIGHT, TILE_GRASS_INNER_BOTTOMLEFT, TILE_GRASS_INNER_TOPLEFT, TILE_GRASS_INNER_TOPRIGHT,
    TILE_GRASS_DOUBLE_VERTICAL, TILE_GRASS_DOUBLE_HORIZONTAL, TILE_GRASS_TRIPLE_TOP, TILE_GRASS_TRIPLE_RIGHT, TILE_GRASS_TRIPLE_BOTTOM, TILE_GRASS_TRIPLE_LEFT, TILE_GRASS_QUADRUPLE,
    TILE_TOTAL
};

enum Forms {
    FORM_WHITE,
    FORM_RED,
    FORM_GREEN,
    FORM_BLUE,
    FORMS_TOTAL
};

struct Resolution {
    int w, h;
};

enum Difficulty {
    DIFFICULTY_EASY = 0,
    DIFFICULTY_MEDIUM = 1,
    DIFFICULTY_HARD = 2,
    DIFFICULTY_EASY_HEALTH = 5,
    DIFFICULTY_MEDIUM_HEALTH = 3,
    DIFFICULTY_HARD_HEALTH = 1
};

struct Save {
    std::string name, slot;
    int level, x, y, form, maxJumps, difficulty, curHealth, maxHealth, keys;
    int collectedKeys[5], unlockedLocks[5];
};

enum Keybinds {
    KEYBINDS_UP,
    KEYBINDS_LEFT,
    KEYBINDS_DOWN,
    KEYBINDS_RIGHT,
    KEYBINDS_JUMP,
    KEYBINDS_TOTAL
};

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern Save save;
extern std::vector<Scene> backStack;
extern Resolution levelDimensions[LEVEL_TOTAL];
extern SDL_Rect tileClips[TILE_TOTAL];
extern Resolution resolutions[5];
extern int curRes;
extern int maxLevel;
extern int keybinds[KEYBINDS_TOTAL];
extern bool quit;
extern void savePersistent();
extern void backCall();
extern void transition(Scene scene);
extern bool checkCollision(SDL_Rect a, SDL_Rect b);
extern bool (*loadMedia)();
extern void (*handleEvent)(SDL_Event*);
extern void (*update)();
extern void (*render)();
extern void (*close)();
extern int tileCount;
extern const int LOGICAL_SCREEN_WIDTH;
extern const int LOGICAL_SCREEN_HEIGHT;
#endif