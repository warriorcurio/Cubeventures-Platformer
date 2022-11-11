#ifndef LSDL_H
#define LSDL_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern bool quit;
extern bool (*loadMedia)();
extern void (*handleEvent)(SDL_Event*);
extern void (*update)();
extern void (*render)();
extern void (*close)();
extern bool mainMenuLoadMedia();
extern void mainMenuHandleEvent(SDL_Event*);
extern void mainMenuUpdate();
extern void mainMenuRender();
extern void mainMenuClose();
extern bool settingsLoadMedia();
extern void settingsHandleEvent(SDL_Event*);
extern void settingsUpdate();
extern void settingsRender();
extern void settingsClose();
extern const int LOGICAL_SCREEN_WIDTH;
extern const int LOGICAL_SCREEN_HEIGHT;
#endif