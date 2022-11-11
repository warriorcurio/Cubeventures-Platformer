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
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
#endif