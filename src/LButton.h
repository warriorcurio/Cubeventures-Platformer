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
        LButton(int x, int y, int w, int h, std::string bgPath, std::string label, SDL_Color labelColour, void (*callback)());
        void handleEvent(SDL_Event* e);
        void render();
    private:
        int mX, mY, mW, mH;
        LTexture mTexture;
        LTexture mLabelTexture;
        ButtonFrames mCurFrame;
        void (*mCallback)();
};