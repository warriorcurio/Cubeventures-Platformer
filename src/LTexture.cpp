#include "LTexture.h"

TTF_Font* font= NULL;

LTexture::LTexture()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}
LTexture::~LTexture()
{
    free();
}
bool LTexture::loadFromFile(std::string path)
{
    free();
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL)
    {
        printf("Unable to load image %s. SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x00, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(newTexture == NULL)
        {
            printf("Unable to create texture from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        else
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    mTexture = newTexture;
    return mTexture != NULL;
}
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, std::string path, int size)
{
    free();
    font = TTF_OpenFont(path.c_str(), size);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
    if(textSurface == NULL)
    {
        printf("Couldn't render text surface: %s\n", TTF_GetError());
    }
    else
    {
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if(mTexture == NULL)
        {
            printf("Unable to create rendered text texture: %s\n", SDL_GetError());
        }
        else
        {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    }
    return mTexture != NULL;
}
bool LTexture::loadFromSVG(std::string svg)
{
    free();
    SDL_RWops* rw = SDL_RWFromConstMem(svg.c_str(), svg.size());
    SDL_Surface* loadedSurface = IMG_Load_RW(rw, 1);
    SDL_Texture* newTexture = NULL;
    if(loadedSurface == NULL)
    {
        printf("Unable to load svg. SDL Error: %s\n", SDL_GetError());
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(newTexture == NULL)
        {
            printf("Unable to create texture from svg. SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    mTexture = newTexture;
    return mTexture != NULL;
}
void LTexture::free()
{
    if(mTexture != NULL)
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}
void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* centre, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    if(clip != NULL)
    {
        SDL_Rect tempClip = {clip->x, clip->y, clip->x + clip->w > mWidth ? mWidth - clip->x:clip->w, clip->y + clip->h > mHeight?mHeight - clip->y:clip->h};
        clip = &tempClip;
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, centre, flip);
}
int LTexture::getWidth()
{
    return mWidth;
}
int LTexture::getHeight()
{
    return mHeight;
}