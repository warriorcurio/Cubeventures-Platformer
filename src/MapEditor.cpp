#include "MapEditor.h"

Resolution editorLevelDimensions[LEVEL_TOTAL] = {
    {4000, 1080},
    {6000, 4000},
    {8000, 2000},
    {4000, 6000},
    {4000, 1080},
    {4000, 1080}
};

SDL_Rect editorCamera = {0, 0, LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT};

int editorTileCount;
int dragType, copyType;
std::vector<CTile*> editorTiles;

bool editorSetTiles()
{
    int x = 0, y = 0;
    char* mapFile = (char*)calloc(17, sizeof(char));
    sprintf(mapFile, "saves/maps/%d.map", save.level + 1);
    std::ifstream map(mapFile);
    if (map.fail()) {
        printf("Unable to load map\n");
        return false;
    }
    for (int i = 0; i < editorTileCount; i++) {
        int tileType = -1;
        map >> tileType;
        if(map.fail()) {
            printf("Error loading map: Unexpected EoF\n");
            return false;
        }
        if(tileType >= 0 && tileType < TILE_TOTAL) {
            editorTiles.push_back(new CTile(x, y, tileType));
        } else {
            printf("Error loading map: Invalid tile type at %d\n", i);
            return false;
        }
        x += CTile::TILE_WIDTH;
        if(x >= editorLevelDimensions[save.level].w) {
            x = 0;
            y += CTile::TILE_HEIGHT;
        }
    }
    x = 0, y = 0;
    for (int i = 0; i < TILE_TOTAL; i++) {
        tileClips[i] = {x, y, CTile::TILE_WIDTH, CTile::TILE_HEIGHT};
        x += CTile::TILE_WIDTH;
        if(x >= tileTexture.getWidth()) {
            x = 0;
            y += CTile::TILE_HEIGHT;
        }
    }
    map.close();
    return true;
}

bool mapEditorLoadMedia()
{
    dragType = -1, copyType = -1;
    tileTexture.loadFromFile("res/tilesDEBUG.png");
    editorTileCount = (editorLevelDimensions[save.level].w / CTile::TILE_WIDTH) * (editorLevelDimensions[save.level].h / CTile::TILE_HEIGHT);
    editorSetTiles();
    return true;
}
void mapEditorHandleEvent(SDL_Event* e)
{
    if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_ESCAPE) {
        quit = true;
    } else if (SDL_GetModState() & KMOD_CTRL && e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_s) {
        char* mapFile = (char*)calloc(17, sizeof(char));
        sprintf(mapFile, "saves/maps/%d.map", save.level + 1);
        std::ofstream map(mapFile);
        for (int i = 0; i < editorTileCount; i++) {
            if (editorTiles[i]->getType() < 10) map << 0;
            map << editorTiles[i]->getType();
            if ((i + 1) % (editorLevelDimensions[save.level].w / CTile::TILE_WIDTH) == 0 && i != editorTileCount - 1) map << "\n";
            else if (i != editorTileCount - 1) map << " ";
        }
        map.close();
    } else if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_RIGHTBRACKET) {
        editorCamera.x = 0;
        editorCamera.y = 0;
        save.level = (save.level + 1) % LEVEL_TOTAL;
        mapEditorClose();
        mapEditorLoadMedia();
    }
    if (e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
            case SDLK_w: editorCamera.y -= 40; break;
            case SDLK_s: editorCamera.y += 40; break;
            case SDLK_a: editorCamera.x -= 40; break;
            case SDLK_d: editorCamera.x += 40; break;
        }
    }
    if (e->type != SDL_MOUSEMOTION && e->type != SDL_MOUSEBUTTONUP && e->type != SDL_KEYUP) return;
        int x, y;
        SDL_GetMouseState(&x, &y);
        float sX, sY;
        SDL_RenderWindowToLogical(gRenderer, x, y, &sX, &sY);
        sX += editorCamera.x;
        sY += editorCamera.y;
        int rowNum = (sY - (int)sY % CTile::TILE_HEIGHT) / CTile::TILE_HEIGHT;
        int colNum = (sX - (int)sX % CTile::TILE_WIDTH) / CTile::TILE_WIDTH;
        int tileNum = (rowNum * editorLevelDimensions[save.level].w / CTile::TILE_WIDTH) + colNum;
    if (e->type == SDL_MOUSEBUTTONUP && !(SDL_GetModState() & KMOD_CTRL) && dragType == -1) {
        if (e->button.button == SDL_BUTTON_LEFT) editorTiles[tileNum]->setType((editorTiles[tileNum]->getType() + 1) % TILE_TOTAL);
        if (e->button.button == SDL_BUTTON_RIGHT) editorTiles[tileNum]->setType((editorTiles[tileNum]->getType() - 1 + TILE_TOTAL) % TILE_TOTAL);
    } else if (e->type == SDL_MOUSEMOTION && dragType != -1) {
        editorTiles[tileNum]->setType(dragType);
    } else if (e->type == SDL_MOUSEBUTTONUP && SDL_GetModState() & KMOD_CTRL && e->button.clicks == 2 && e->button.button == SDL_BUTTON_LEFT) {
        if (dragType == -1) dragType = editorTiles[tileNum]->getType();
        else dragType = -1;
    } else if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
        copyType = editorTiles[tileNum]->getType();
    } else if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL && copyType != -1) {
        editorTiles[tileNum]->setType(copyType);
    } else if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_h && SDL_GetModState() & KMOD_CTRL && copyType != -1) {
        for (int i = 0; i < editorTileCount; i++) {
            if (editorTiles[i]->getType() >= copyType) editorTiles[i]->setType(editorTiles[i]->getType() + 1);
        }
        copyType++;
    } else if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_j && SDL_GetModState() & KMOD_CTRL && copyType != -1) {
        copyType--;
        for (int i = 0; i < editorTileCount; i++) {
            if (editorTiles[i]->getType() >= copyType) editorTiles[i]->setType(editorTiles[i]->getType() - 1);
        }
    } else if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_i && SDL_GetModState() & KMOD_CTRL) {
        printf("%dx, %dy, %dt; ", (int)sX, (int)sY, tileNum);
    }
}
void mapEditorUpdate()
{
    if(editorCamera.x < 0) editorCamera.x = 0;
    if(editorCamera.y < 0) editorCamera.y = 0;
    if(editorCamera.x > levelDimensions[save.level].w - editorCamera.w) editorCamera.x = levelDimensions[save.level].w - editorCamera.w;
    if(editorCamera.y > levelDimensions[save.level].h - editorCamera.h) editorCamera.y = levelDimensions[save.level].h - editorCamera.h;
}
void mapEditorRender()
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xAA, 0xFF);
    SDL_RenderClear(gRenderer);
    for (int i = 0; i < editorTileCount; i++) {
        editorTiles[i]->render(editorCamera);
    }
}
void mapEditorClose()
{
    for (int i = 0; i < editorTileCount; i++) {
        if (editorTiles[i]) delete editorTiles[i];
    }
    editorTiles.clear();
}