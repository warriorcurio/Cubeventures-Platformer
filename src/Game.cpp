#include "Game.h"

Resolution levelDimensions[LEVEL_TOTAL] = {
    {4000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080}
};
SDL_Point levelStartPositions[LEVEL_TOTAL] = {
    {0, 1020},
    {0, 1020},
    {0, 1020},
    {0, 1020},
    {0, 1020},
    {0, 1020},
    {0, 1020},
    {0, 1020},
    {0, 1020},
    {0, 1020}
};

SDL_Rect camera = {0, 0, LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT};

float timeStep;
Uint32 timeTicks;

int tileCount;
std::vector<LTile*> tiles;
SDL_Rect tileClips[TILE_TOTAL];
LTexture tileTexture;

LTexture keyTexture;

LPlayer* player;

LButton* gameButtons[GAME_BUTTON_TOTAL];

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
    if(a.y + a.h <= b.y) return false;
    if(b.y + b.h <= a.y) return false;
    if(a.x + a.w <= b.x) return false;
    if(b.x + b.w <= a.x) return false;
    return true;
}
bool setTiles()
{
    int x = 0, y = 0;
    char* mapFile = (char*)calloc(18, sizeof(char));
    sprintf(mapFile, "saves/maps/%d.map", save.level);
    std::ifstream map(mapFile);
    if (map.fail()) {
        printf("Unable to load map\n");
        return false;
    }
    for (int i = 0; i < tileCount; i++) {
        int tileType = -1;
        map >> tileType;
        if(map.fail()) {
            printf("Error loading map: Unexpected EoF\n");
            return false;
        }
        if(tileType >= 0 && tileType < TILE_TOTAL) {
            tiles.push_back(new LTile(x, y, tileType));
        } else {
            printf("Error loading map: Invalid tile type at %d\n", i);
            return false;
        }
        x += LTile::TILE_WIDTH;
        if(x >= levelDimensions[save.level - 1].w) {
            x = 0;
            y += LTile::TILE_HEIGHT;
        }
    }
    x = 0, y = 0;
    for (int i = 0; i < TILE_TOTAL; i++) {
        tileClips[i] = {x, y, LTile::TILE_WIDTH, LTile::TILE_HEIGHT};
        x += LTile::TILE_WIDTH;
        if(x >= tileTexture.getWidth()) {
            x = 0;
            y += LTile::TILE_HEIGHT;
        }
    }
    map.close();
    for (int i = 0; i < 5; i++) {
        if (save.collectedKeys[i]) tiles[save.collectedKeys[i]]->setType(TILE_EMPTY);
    }
    for (int i = 0; i < 5; i++) {
        if (save.collectedKeys[i]) tiles[save.unlockedLocks[i]]->setType(TILE_EMPTY);
    }
    return true;
}

void nextLevel()
{
    for (int i = 0; i < tileCount; i++) {
        if (tiles[i]) delete tiles[i];
    }
    tiles.clear();
    save.level++;
    save.x = levelStartPositions[save.level].x;
    save.y = levelStartPositions[save.level].y;
    save.keys = 0;
    for (int i = 0; i < 5; i++) {
        save.collectedKeys[i] = 0;
        save.unlockedLocks[i] = 0;
    }
    if (save.level > maxLevel) {
        maxLevel = save.level;
        savePersistent();
    }
    char* slotFile = (char*)calloc(20, sizeof(char));
    sprintf(slotFile, "saves/save_%s.bin", save.slot.c_str());
    SDL_RWops* writeFile = SDL_RWFromFile(slotFile, "wb");
    SDL_RWwrite(writeFile, &save, sizeof(Save), 1);
    SDL_RWclose(writeFile);
    player->setPos(save.x, save.y);
    timeTicks = SDL_GetTicks();
    tileCount = (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH) * (levelDimensions[save.level - 1].h / LTile::TILE_HEIGHT);
    setTiles();
}

bool gameLoadMedia()
{
    keyTexture.loadFromFile("res/key.png");
    timeTicks = SDL_GetTicks();
    player = new LPlayer(save.x, save.y);
    tileTexture.loadFromFile("res/tilesDEBUG.png");
    tileCount = (levelDimensions[save.level - 1].w / LTile::TILE_WIDTH) * (levelDimensions[save.level - 1].h / LTile::TILE_HEIGHT);
    setTiles();
    return true;
}
void gameHandleEvent(SDL_Event* e)
{
    if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_ESCAPE) {
        transition(SCENE_PAUSE);
    }
    for (int i = 0; i < GAME_BUTTON_TOTAL; i++) {
        if (gameButtons[i]) gameButtons[i]->handleEvent(e);
    }
    player->handleEvent(e);
}
void gameUpdate()
{
    timeStep = (SDL_GetTicks() - timeTicks) / 1000.f;
    player->move(tiles, timeStep);
    for (int i = 0; i < tileCount; i++) {
        tiles[i]->updateTiles(timeStep);
    }
    timeTicks = SDL_GetTicks();
    player->setCamera(camera);
    player->checkItemCollisions(tiles);
}
void gameRender()
{
    SDL_SetRenderDrawColor(gRenderer, 69, 69, 69, 0xFF);
    SDL_RenderClear(gRenderer);
    for (int i = 0; i < tileCount; i++) {
        tiles[i]->render(camera);
    }
    player->render(camera);
    for (int i = 0; i < player->getKeys(); i++) {
        keyTexture.render(i * keyTexture.getWidth(), 0);
    }
    for (int i = 0; i < GAME_BUTTON_TOTAL; i++) {
        if (gameButtons[i]) gameButtons[i]->render();
    }
}
void gameClose()
{
    for (int i = 0; i < GAME_BUTTON_TOTAL; i++) {
        if (gameButtons[i]) delete gameButtons[i];
    }
    for (int i = 0; i < tileCount; i++) {
        if (tiles[i]) delete tiles[i];
    }
    tiles.clear();
    delete player;
    tileTexture.free();
    keyTexture.free();
}