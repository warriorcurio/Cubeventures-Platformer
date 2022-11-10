#include "LButton.h"

LButton::LButton(int x, int y, int fontSize, SDL_Color bgColours[3], std::string label, SDL_Color labelColour, void (*callback)())
{
    mX = x;
    mY = y;
    mLabelTexture.loadFromRenderedText(label.c_str(), labelColour, "res/font.TTF", fontSize);
    mW = mLabelTexture.getWidth() + 10;
    mH = mLabelTexture.getHeight() + 10;
    char svg[300];
    sprintf(svg, "<svg><rect y='0' width='%d' height='%d' style='fill:rgb(%d,%d,%d)' /><rect y='%d' width='%d' height='%d' style='fill:rgb(%d,%d,%d)' /><rect y='%d' width='%d' height='%d' style='fill:rgb(%d,%d,%d)' /></svg>", mW, mH, (int)bgColours[0].r, (int)bgColours[0].g, (int)bgColours[0].b, mH, mW, mH, (int)bgColours[1].r, (int)bgColours[1].g, (int)bgColours[1].b, mH*2, mW, mH, (int)bgColours[2].r, (int)bgColours[2].g, (int)bgColours[2].b);
    mTexture.loadFromSVG(svg);
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
            if (mCurFrame == BUTTON_MOUSE_DOWN) break;
            mCurFrame = BUTTON_MOUSE_OVER;
            break;
        case SDL_MOUSEBUTTONDOWN:
            mCurFrame = BUTTON_MOUSE_DOWN;
            break;
        case SDL_MOUSEBUTTONUP:
            if (mCurFrame != BUTTON_MOUSE_DOWN) break;
            mCurFrame = BUTTON_MOUSE_OVER;
            mCallback();
            break;
    }
}
void LButton::render()
{
    SDL_Rect clip = {0, mCurFrame * mTexture.getHeight() / 3, mTexture.getWidth(), mTexture.getHeight() / 3};
    mTexture.render(mX, mY, &clip);
    mTexture.render(500, 500);
    mLabelTexture.render(mX + 5, mY + 5);
}