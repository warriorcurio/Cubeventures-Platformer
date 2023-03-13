#ifndef CTEXTURE_H
#define CTEXTURE_H
#include "LSDL.h"

class CTexture
{
    public:
        CTexture(); //initialiser
        void loadFromFile(std::string path); //loads an image from its path
        void loadFromRenderedText(std::string textureText, SDL_Color textColor, int size); //loads text
        void loadFromSVG(std::string svg); //loads from SVG text
        void free(); //frees the texture
        void setColour(Uint8 r, Uint8 g, Uint8 b); //sets the colour of the texture
        void render(int x, int y, SDL_Rect* clip = NULL);
        int getWidth();
        int getHeight();
    private:
        SDL_Texture* mTexture;
        int mWidth, mHeight;
};

extern CTexture menuBackground;
extern CTexture menuOverlay;
extern CTexture saveHearts;
#endif