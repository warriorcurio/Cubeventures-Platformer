#include "NameSave.h"

std::string inputName;

CButton* nameSaveButtons[NAMESAVE_BUTTON_TOTAL];

CTexture textNameSave;
CTexture textEnter;
CTexture textInputName;

bool nameSaveLoadMedia()
{
    inputName = "";
    SDL_StartTextInput();
    textNameSave.loadFromRenderedText("Type Your Name", SDL_Color{0xFF, 0xFF, 0xFF}, 40);
    textEnter.loadFromRenderedText("Press ENTER To Submit", SDL_Color{0xFF, 0xFF, 0xFF}, 40);
    nameSaveButtons[NAMESAVE_BUTTON_BACK]  = new CButton(10, 1020, 40, "Back", &backCall);
    return true;
}
void nameSaveHandleEvent(SDL_Event* e)
{
    if (curButton != -1) nameSaveButtons[curButton]->setSelected(false);
    menuHandleButtonSwitching(e, NAMESAVE_BUTTON_TOTAL);
    if (curButton != -1) nameSaveButtons[curButton]->setSelected(true);
    for (int i = 0; i < NAMESAVE_BUTTON_TOTAL; i++) {
        if (nameSaveButtons[i]) nameSaveButtons[i]->handleEvent(e);
    }
    if (e->type == SDL_KEYDOWN) {
        //enters the player's inputted name to the save and transitions to the next screen
        if(e->key.keysym.sym == SDLK_RETURN && inputName != "") {
            strcpy(save.name, inputName.c_str());
            backStack.push_back(SCENE_NAMESAVE);
            transition(SCENE_CREATESAVE);
        } else if (e->key.keysym.sym == SDLK_BACKSPACE && inputName != "") inputName.pop_back(); //removes the end character when the player backspaces
    } else if(e->type == SDL_TEXTINPUT) { //enters the player's inputted character
        inputName += e->text.text;
        if (inputName.length() > 6) inputName.pop_back(); //stops the player from entering more than six characters
    }
}
void nameSaveUpdate()
{
    //updates the texture displaying the player's input
    //textures can't be created from an empty string
    //if no text has been inputted then the text is rendered as a space, which is invisible
    if (inputName != "") textInputName.loadFromRenderedText(inputName.c_str(), SDL_Color{0xFF, 0xFF, 0xFF}, 60);
    else textInputName.loadFromRenderedText(" ", SDL_Color{0xFF, 0xFF, 0xFF}, 60);
}
void nameSaveRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    menuBackground.render(0, 0);
    menuOverlay.render(300, 300);
    textNameSave.render((LOGICAL_SCREEN_WIDTH - textNameSave.getWidth()) / 2, 325);
    textEnter.render((LOGICAL_SCREEN_WIDTH - textEnter.getWidth()) / 2, 715);
    textInputName.render((LOGICAL_SCREEN_WIDTH - textInputName.getWidth()) / 2, (LOGICAL_SCREEN_HEIGHT - textInputName.getHeight()) / 2);
    for (int i = 0; i < NAMESAVE_BUTTON_TOTAL; i++) {
        if (nameSaveButtons[i]) nameSaveButtons[i]->render();
    }
}
void nameSaveClose()
{
    SDL_StopTextInput();
    for (int i = 0; i < NAMESAVE_BUTTON_TOTAL; i++) {
        if (nameSaveButtons[i]) delete nameSaveButtons[i];
    }
    textNameSave.free();
}