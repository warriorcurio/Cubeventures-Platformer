#include "LButton.h"

LButton::LButton(int x, int y, int fontSize, std::string bgColours[3], std::string label, SDL_Color labelColour, void (*callback)(), int w, int h)
{
    mX = x;
    mY = y;
    mLabelTexture.loadFromRenderedText(label.c_str(), labelColour, "res/04b.TTF", fontSize);
    mW = mLabelTexture.getWidth() + 10;
    mH = mLabelTexture.getHeight() + 10;
    if (w > 0) {
        mW = w;
        mH = h;
    }
    mClickable = true;
    mCurFrame = BUTTON_MOUSE_OUT;
    char svg[300];
    sprintf(svg, "<svg width='%d' height='%d'><rect y='0' rx='5' ry='5' width='%d' height='%d' style='fill:%s' /><rect y='%d' rx='5' ry='5' width='%d' height='%d' style='fill:%s' /><rect y='%d' rx='5' ry='5' width='%d' height='%d' style='fill:%s' /><rect y='%d' rx='5' ry='5' width='%d' height='%d' style='fill:#3F3F3F' /></svg>", mW, mH*4, mW, mH, bgColours[0].c_str(), mH, mW, mH, bgColours[1].c_str(), mH*2, mW, mH, bgColours[2].c_str(), mH*3, mW, mH);
    mTexture.loadFromSVG(svg);
    mCallback = callback;
}
LButton::~LButton()
{
    mTexture.free();
    mLabelTexture.free();
}
void LButton::handleEvent(SDL_Event* e)
{
    if (!(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) || !mClickable) {
        return;
    }
    int x, y;
    SDL_GetMouseState(&x, &y);
    float sX, sY;
    SDL_RenderWindowToLogical(gRenderer, x, y, &sX, &sY);
    bool inside = true;
    (
        sX < mX ||
        sX > mX + mW ||
        sY < mY ||
        sY > mY + mH
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
    SDL_Rect clip;
    if (mClickable) {
        clip = {0, mCurFrame * mTexture.getHeight() / 4, mTexture.getWidth(), mTexture.getHeight() / 4};
    } else {
        clip = {0, 3 * mTexture.getHeight() / 4, mTexture.getWidth(), mTexture.getHeight() / 4};
    }
    mTexture.render(mX, mY, &clip);
    mLabelTexture.render(mX + (mW - mLabelTexture.getWidth()) / 2, mY + (mH - mLabelTexture.getHeight()) / 2);
}
void LButton::setPos(int x, int y)
{
    mX = x;
    mY = y;
}
void LButton::setBGFromPath(std::string path)
{
    mTexture.free();
    mTexture.loadFromFile(path.c_str());
}
void LButton::setLabelFromPath(std::string path)
{
    mLabelTexture.free();
    mLabelTexture.loadFromFile(path.c_str());
}
void LButton::setClickable(bool isClickable)
{
    mClickable = isClickable;
}
int LButton::getX()
{
    return mX;
}
int LButton::getY()
{
    return mY;
}
int LButton::getW()
{
    return mW;
}
int LButton::getH()
{
    return mH;
}