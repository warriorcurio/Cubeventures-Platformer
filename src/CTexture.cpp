#include "CTexture.h"

TTF_Font* font= NULL;

CTexture::CTexture()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}
void CTexture::loadFromFile(std::string path)
{
    free();
    SDL_Surface* loadedSurface = IMG_Load(path.c_str()); //loads an image from its provided file path
    mTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface); //creates the texture from the loaded image
    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;
    SDL_FreeSurface(loadedSurface); //frees up the memory being used for loaded image
}
void CTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, int size)
{
    free();
    font = TTF_OpenFont("res/04b.TTF", size); //loads the font from the res folder at the passed in size
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor); //creates a surface from the font as well as the text colour parameter
    mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface); //creates the texture from the loaded text
    mWidth = textSurface->w;
    mHeight = textSurface->h;
    SDL_FreeSurface(textSurface); //frees up the memory being used for loaded text
}
void CTexture::loadFromSVG(std::string svg)
{
    free();
    SDL_RWops* rw = SDL_RWFromConstMem(svg.c_str(), svg.size()); //reads the svg code
    SDL_Surface* loadedSurface = IMG_Load_RW(rw, 1); //creates a surface from the svg code
    mTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface); //creates the texture from the loaded svg code
    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;
    SDL_FreeSurface(loadedSurface); //frees up the memory being used for loaded svg
}
void CTexture::free()
{
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture); //destroys the texture and frees space in memory
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}
void CTexture::setColour(Uint8 r, Uint8 g, Uint8 b)
{
    SDL_SetTextureColorMod(mTexture, r, g, b); //sets the texture's colour
}
void CTexture::render(int x, int y, SDL_Rect* clip)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    //if there is a clip, the rendering target will have to be the same size as it
    if (clip) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad); //copies the texture to the renderer
}
int CTexture::getWidth()
{
    return mWidth;
}
int CTexture::getHeight()
{
    return mHeight;
}