#ifndef LSDL_H
#define LSDL_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cstring>
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
    TILE_GHOST_T_D,
    TILE_GHOST_F_D,
    TILE_STEAM,
    TILE_STEAMVENT,
    TILE_ANTIGRAV_SOURCEMOON, TILE_ANTIGRAV_SOURCEVOID,
    TILE_ANTIGRAV_TOPLEFT, TILE_ANTIGRAV_TOPMIDDLE, TILE_ANTIGRAV_TOPRIGHT, TILE_ANTIGRAV_MIDDLELEFT, TILE_ANTIGRAV_MIDDLE, TILE_ANTIGRAV_MIDDLERIGHT, TILE_ANTIGRAV_BOTTOMLEFT, TILE_ANTIGRAV_BOTTOMMIDDLE, TILE_ANTIGRAV_BOTTOMRIGHT,
    TILE_KEY,
    TILE_LOCK_D,
    TILE_MEDAL,
    TILE_EXIT,
    TILE_SPIKEUP, TILE_SPIKERIGHT, TILE_SPIKEDOWN, TILE_SPIKELEFT,
    TILE_CAVESPIKEUP, TILE_CAVESPIKERIGHT, TILE_CAVESPIKEDOWN, TILE_CAVESPIKELEFT,
    TILE_HELLSPIKEUP, TILE_HELLSPIKERIGHT, TILE_HELLSPIKEDOWN, TILE_HELLSPIKELEFT,
    TILE_WATERUP, TILE_WATER, TILE_LAVAUP, TILE_LAVA,
    TILE_EMPTY,
    TILE_GHOST_T,
    TILE_GHOST_F,
    TILE_LOCK,
    TILE_GRASS_TOPLEFT, TILE_GRASS_TOPMIDDLE, TILE_GRASS_TOPRIGHT, TILE_GRASS_MIDDLELEFT, TILE_GRASS_MIDDLE, TILE_GRASS_MIDDLERIGHT, TILE_GRASS_BOTTOMLEFT, TILE_GRASS_BOTTOMMIDDLE, TILE_GRASS_BOTTOMRIGHT,
    TILE_GRASS_INNER_BOTTOMRIGHT, TILE_GRASS_INNER_BOTTOMLEFT, TILE_GRASS_INNER_TOPLEFT, TILE_GRASS_INNER_TOPRIGHT,
    TILE_GRASS_DOUBLE_VERTICAL, TILE_GRASS_DOUBLE_HORIZONTAL, TILE_GRASS_TRIPLE_TOP, TILE_GRASS_TRIPLE_RIGHT, TILE_GRASS_TRIPLE_BOTTOM, TILE_GRASS_TRIPLE_LEFT, TILE_GRASS_QUADRUPLE,
    TILE_STONE,
    TILE_JUNGLE_TOPLEFT, TILE_JUNGLE_TOPMIDDLE, TILE_JUNGLE_TOPRIGHT, TILE_JUNGLE_MIDDLELEFT, TILE_JUNGLE_MIDDLE, TILE_JUNGLE_MIDDLERIGHT, TILE_JUNGLE_BOTTOMLEFT, TILE_JUNGLE_BOTTOMMIDDLE, TILE_JUNGLE_BOTTOMRIGHT,
    TILE_JUNGLE_INNER_BOTTOMRIGHT, TILE_JUNGLE_INNER_BOTTOMLEFT, TILE_JUNGLE_INNER_TOPLEFT, TILE_JUNGLE_INNER_TOPRIGHT,
    TILE_JUNGLE_DOUBLE_VERTICAL, TILE_JUNGLE_DOUBLE_HORIZONTAL, TILE_JUNGLE_TRIPLE_TOP, TILE_JUNGLE_TRIPLE_RIGHT, TILE_JUNGLE_TRIPLE_BOTTOM, TILE_JUNGLE_TRIPLE_LEFT, TILE_JUNGLE_QUADRUPLE,
    TILE_BOUNCE,
    TILE_BLUESTONE,
    TILE_ICE,
    TILE_ASH,
    TILE_OBSIDIAN,
    TILE_MOONSTONE,
    TILE_VOIDSTONE,
    TILE_TOTAL
};

enum Forms {
    FORM_WHITE,
    FORM_RED,
    FORM_GREEN,
    FORM_BLUE,
    FORM_RAINBOW,
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
    char name[6];
    int level, x, y, form, maxJumps, difficulty, curHealth, maxHealth, keys, deaths, score, charge;
    bool finishedGame;
    char slot[5];
    float totalTime, chapterTime;
    int collectedKeys[5], unlockedLocks[5];
    bool collectedMedals[LEVEL_TOTAL];
};

enum Keybinds {
    KEYBINDS_UP,
    KEYBINDS_LEFT,
    KEYBINDS_DOWN,
    KEYBINDS_RIGHT,
    KEYBINDS_JUMP,
    KEYBINDS_ABILITY,
    KEYBINDS_TOTAL
};

enum ProjectileTypes {
    PROJECTILE_HEART,
    PROJECTILE_SHIELD,
    PROJECTILE_CHARGER,
    PROJECTILE_DAMAGEBALL,
    PROJECTILE_BUTTON_TILECHANGE,
    PROJECTILE_BUTTON_TILETOGGLE,
    PROJECTILE_TELEPORTER,
    PROJECTILE_SAVER,
    PROJECTILE_TEXTDISPLAYER,
    PROJECTILE_BOUNCEBLOCK,
    PROJECTILE_ANTIGRAV,
    PROJECTILE_TOTAL
};

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern SDL_GameController* gController;
extern int curButton;
extern Save save;
extern std::vector<Scene> backStack;
extern SDL_Rect saveHeartClips[3];
extern std::string saveFileNames[3];
extern std::string saveButtonPaths[LEVEL_TOTAL];
extern std::string difficultyNames[3];
extern Save saveSlots[3];
extern Resolution levelDimensions[LEVEL_TOTAL];
extern SDL_Point levelStartPositions[LEVEL_TOTAL];
extern SDL_Rect tileClips[TILE_TOTAL];
extern Resolution resolutions[5];
extern int curRes;
extern int maxLevel;
extern int maxScore;
extern bool hasEverFinishedGame;
extern std::string bgNames[LEVEL_TOTAL];
extern std::string bgParallaxNames[LEVEL_TOTAL];
extern int keybinds[KEYBINDS_TOTAL];
extern bool quit;
extern void menuHandleButtonSwitching(SDL_Event* e, int totalButtons);
extern void changeControllerRGB();
extern void setWindowIcon(int iconNum);
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
extern const int JOYSTICK_DEAD_ZONE;
#endif