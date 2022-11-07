#ifndef LSDL_H
#define LSDL_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern void (*update)();
extern void (*render)();
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
#endif