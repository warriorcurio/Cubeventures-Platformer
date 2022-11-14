#ifndef LSDL_H
#define LSDL_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>

enum Scene {
    SCENE_MAINMENU,
    SCENE_SETTINGS,
    SCENE_LEVELSELECT,
    SCENE_TOTAL
};

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern bool quit;
extern void transition(Scene scene);
extern bool (*loadMedia)();
extern void (*handleEvent)(SDL_Event*);
extern void (*update)();
extern void (*render)();
extern void (*close)();
extern const int LOGICAL_SCREEN_WIDTH;
extern const int LOGICAL_SCREEN_HEIGHT;
#endif