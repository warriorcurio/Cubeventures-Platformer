#ifndef LBUTTON_H
#define LBUTTON_H
#include "LSDL.h"
#include "LTexture.h"

enum ButtonFrames
{
    BUTTON_MOUSE_OUT,
    BUTTON_MOUSE_OVER,
    BUTTON_MOUSE_DOWN
};

class LButton
{
    public:
        LButton(int x, int y, int  fontSize, std::string bgColours[3], std::string label, SDL_Color labelColour, void (*callback)(), int w = 0, int h = 0);
        ~LButton();
        void handleEvent(SDL_Event* e);
        void render();
        void setPos(int x, int y);
        void setBGFromPath(std::string path);
        void setLabelFromPath(std::string path);
        void setClickable(bool isClickable);
        void setSelected(bool isSelected);
        int getX();
        int getY();
        int getW();
        int getH();
    private:
        int mX, mY, mW, mH;
        bool mClickable, mIsSelected;
        LTexture mTexture;
        LTexture mLabelTexture;
        ButtonFrames mCurFrame;
        void (*mCallback)();
};
#endif