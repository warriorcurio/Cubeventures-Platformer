#include "CButton.h"

CButton::CButton(int x, int y, int fontSize, std::string label, void (*callback)(), int w, int h)
{
    mX = x;
    mY = y;
    mLabelTexture.loadFromRenderedText(label.c_str(), SDL_Color{0xFF, 0xFF, 0xFF}, fontSize);
    mW = mLabelTexture.getWidth() + 10;
    mH = mLabelTexture.getHeight() + 10;
    //sets the width and height if they were passed in manually
    if (w > 0) mW = w;
    if (h > 0) mH = h;
    mClickable = true;
    mIsSelected = false;
    mCurFrame = BUTTON_MOUSE_OUT;
    char svg[350];
    //svg code to create the different frames of the button
    sprintf(svg, "<svg width='%d' height='%d'>"
                 "<rect y='0' rx='5' ry='5' width='%d' height='%d' style='fill:#006F00' />"
                 "<rect y='%d' rx='5' ry='5' width='%d' height='%d' style='fill:#003F00' />"
                 "<rect y='%d' rx='5' ry='5' width='%d' height='%d' style='fill:#003F3F' />"
                 "<rect y='%d' rx='5' ry='5' width='%d' height='%d' style='fill:#3F3F3F' />"
                 "</svg>", mW, mH*4, mW, mH, mH, mW, mH, mH*2, mW, mH, mH*3, mW, mH);
    mTexture.loadFromSVG(svg);
    mCallback = callback;
}
CButton::~CButton()
{
    mTexture.free();
    mLabelTexture.free();
}
void CButton::handleEvent(SDL_Event* e)
{
    if (!mClickable) return;
    //controller inputs
    if (mIsSelected && ((e->type == SDL_JOYBUTTONDOWN && e->jbutton.button == SDL_CONTROLLER_BUTTON_A) || (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_SPACE))) {
        mCurFrame = BUTTON_MOUSE_DOWN;
    } else if (mIsSelected && ((e->type == SDL_JOYBUTTONUP && e->jbutton.button == SDL_CONTROLLER_BUTTON_A) || (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_SPACE))) {
        mCurFrame = BUTTON_MOUSE_OVER;
        mCallback(); //calls the callback procedure when the player releases the controller button
        Mix_PlayChannel(SFX_BUTTON, sfx[SFX_BUTTON], 0);
    }
    if (!(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)) return;
    curButton = -1;
    int x, y;
    SDL_GetMouseState(&x, &y); //places the mouse position into x and y
    float sX, sY;
    SDL_RenderWindowToLogical(gRenderer, x, y, &sX, &sY); //converts the mouse position to a relative position in the window
    bool inside = !(
        sX < mX || //case: mouse is on the left of the button
        sX > mX + mW || //case: mouse is on the right of the button
        sY < mY || //case: mouse is above the button
        sY > mY + mH //case: mouse is below the button
    );
    if (!inside) {
        mCurFrame = BUTTON_MOUSE_OUT;
        return;
    }
    switch (e->type) {
        case SDL_MOUSEMOTION:
            if (mCurFrame == BUTTON_MOUSE_DOWN) break;
            mCurFrame = BUTTON_MOUSE_OVER;
            break;
        case SDL_MOUSEBUTTONDOWN: mCurFrame = BUTTON_MOUSE_DOWN; break;
        case SDL_MOUSEBUTTONUP:
            if (mCurFrame != BUTTON_MOUSE_DOWN) break;
            mCurFrame = BUTTON_MOUSE_OVER;
            mCallback(); //calls the callback procedure when the player releases the mouse button
            Mix_PlayChannel(SFX_BUTTON, sfx[SFX_BUTTON], 0);
            break;
    }
}
void CButton::render()
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
void CButton::setPos(int x, int y)
{
    mX = x;
    mY = y;
}
void CButton::setBGFromPath(std::string path)
{
    mTexture.free();
    mTexture.loadFromFile(path.c_str());
}
void CButton::setBGFromSVG(std::string bgColours[3])
{
    mTexture.free();
    char svg[350];
    //svg code to create the different frames of the button
    sprintf(svg, "<svg width='%d' height='%d'>"
    "<rect y='0' rx='5' ry='5' width='%d' height='%d' style='fill:%s' />"
    "<rect y='%d' rx='5' ry='5' width='%d' height='%d' style='fill:%s' />"
    "<rect y='%d' rx='5' ry='5' width='%d' height='%d' style='fill:%s' />"
    "<rect y='%d' rx='5' ry='5' width='%d' height='%d' style='fill:#3F3F3F' />"
    "</svg>", mW, mH*4, mW, mH, bgColours[0].c_str(), mH, mW, mH, bgColours[1].c_str(), mH*2, mW, mH, bgColours[2].c_str(), mH*3, mW, mH);
    mTexture.loadFromSVG(svg);
}
void CButton::setLabelFromPath(std::string path)
{
    mLabelTexture.free();
    mLabelTexture.loadFromFile(path.c_str());
}
void CButton::setClickable(bool isClickable)
{
    mClickable = isClickable;
}
void CButton::setSelected(bool isSelected)
{
    if (!mClickable) return;
    mIsSelected = isSelected;
    if (isSelected) mCurFrame = BUTTON_MOUSE_OVER;
    else mCurFrame = BUTTON_MOUSE_OUT;
}
int CButton::getX()
{
    return mX;
}
int CButton::getY()
{
    return mY;
}
int CButton::getW()
{
    return mW;
}
int CButton::getH()
{
    return mH;
}
ButtonFrames CButton::getFrame()
{
    return mCurFrame;
}