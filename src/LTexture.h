#ifndef LTEXTURE_H
#define LTEXTURE_H
#include "LSDL.h"

class LTexture
{
    public:
        LTexture();
        ~LTexture();
        bool loadFromFile(std::string path);
        bool loadFromRenderedText(std::string textureText, SDL_Color textColor, std::string path, int size);
        void free();
        void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* centre = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
        int getWidth();
        int getHeight();
    private:
        SDL_Texture* mTexture;
        int mWidth, mHeight;
};
#endif