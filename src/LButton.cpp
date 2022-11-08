#include "LButton.h"

LButton::LButton(int x, int y, int w, int h, std::string bgPath, std::string label, SDL_Color labelColour, void (*callback)())
{
    mX = x;
    mY = y;
    mW = w;
    mH = h;
    mTexture.loadFromFile(bgPath.c_str());
    mLabelTexture.loadFromRenderedText(label.c_str(), labelColour, "res/font.TTF", h - 10);
    mCallback = callback;
}
void LButton::handleEvent(SDL_Event* e)
{
    if (!(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)) {
        return;
    }
    int x, y;
    SDL_GetMouseState(&x, &y);
    bool inside = true;
    (
        x < mX ||
        x > mX + mW ||
        y < mY ||
        y > mY + mH
    )?inside = false:true;
    if(!inside)
    {
        mCurFrame = BUTTON_MOUSE_OUT;
        return;
    }
    switch(e->type)
    {
        case SDL_MOUSEMOTION:
            mCurFrame = BUTTON_MOUSE_OVER;
            break;
        case SDL_MOUSEBUTTONDOWN:
            mCurFrame = BUTTON_MOUSE_DOWN;
            break;
        case SDL_MOUSEBUTTONUP:
            mCurFrame = BUTTON_MOUSE_OVER;
            mCallback();
            break;
    }
}
void LButton::render()
{
    SDL_Rect clip = {0, mCurFrame * mTexture.getHeight() / 3, mTexture.getWidth(), mTexture.getHeight() / 3};
    mTexture.render(mX, mY, &clip);
    mLabelTexture.render(mX + 5, mY + 5);
}