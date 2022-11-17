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
    SCENE_TOTAL
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
    std::string name;
    int slot, level, x, y, difficulty, curHealth;
};

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern Save save;
extern std::vector<Scene> backStack;
extern bool quit;
extern void backCall();
extern void transition(Scene scene);
extern bool (*loadMedia)();
extern void (*handleEvent)(SDL_Event*);
extern void (*update)();
extern void (*render)();
extern void (*close)();
extern const int LOGICAL_SCREEN_WIDTH;
extern const int LOGICAL_SCREEN_HEIGHT;
#endif