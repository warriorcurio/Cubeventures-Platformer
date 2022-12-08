#include "Game.h"

Resolution levelDimensions[LEVEL_TOTAL] = {
    {4000, 1080},
    {1920, 4000},
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
LTexture heartTexture;
SDL_Rect heartClips[3] = {{0, 0, 80, 80}, {0, 80, 80, 80}, {0, 160, 80, 80}};
LTexture heartTwinkleTexture;
SDL_Rect heartTwinkleClips[6] = {{0, 0, 9, 9}, {9, 0, 9, 9}, {18, 0, 9, 9}, {0, 9, 9, 9}, {9, 9, 9, 9}, {18, 9, 9, 9}};
int heartTwinkleNum = -1;
bool isCreatingNewTwinkle;
std::vector<int> heartTwinkleFrames;
std::vector<SDL_Point> heartTwinklePositions;

LTexture bgTexture;
LTexture bgPTexture;
float parallaxOffset;

LPlayer* player;

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

void setLevel(int level)
{
    for (int i = 0; i < tileCount; i++) {
        if (tiles[i]) delete tiles[i];
    }
    tiles.clear();
    parallaxOffset = -1 * (rand() % bgPTexture.getWidth());
    save.level = level;
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
    bgTexture.loadFromFile("res/bgONE.png");
    bgPTexture.loadFromFile("res/bgONE_P.png");
    parallaxOffset = -1 * (rand() % bgPTexture.getWidth());
    keyTexture.loadFromFile("res/key.png");
    heartTexture.loadFromFile("res/hearts.png");
    heartTwinkleTexture.loadFromFile("res/heartTwinkle.png");
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
    if (parallaxOffset < -bgPTexture.getWidth()) parallaxOffset += bgPTexture.getWidth();
}
void gameRender()
{
    SDL_SetRenderDrawColor(gRenderer, 69, 69, 69, 0xFF);
    SDL_RenderClear(gRenderer);
    bgTexture.render(0, 0);
    bgPTexture.render((int)parallaxOffset, 0);
    bgPTexture.render((int)parallaxOffset + bgPTexture.getWidth(), 0);
    for (int i = 0; i < tileCount; i++) {
        tiles[i]->render(camera);
    }
    player->render(camera);
    for (int i = 0; i < save.maxHealth; i++) {
        heartTexture.render(80 * i, 0, &heartClips[0]);
        if (i < player->getHealth() && save.difficulty != DIFFICULTY_EASY) heartTexture.render(80 * i, 0, &heartClips[1]);
        if (i < player->getHealth() && save.difficulty == DIFFICULTY_EASY) heartTexture.render(80 * i, 0, &heartClips[2]);
        if (i == player->getHealth() && save.difficulty == DIFFICULTY_EASY) {
            isCreatingNewTwinkle = (rand() % 20) == 0;
            heartTwinkleNum = i;
            SDL_Rect regenClip = heartClips[2];
            regenClip.h = (int)(safePositionTimeTimerSeconds * heartClips[2].h / safePositionTimeSeconds);
            regenClip.y = heartClips[2].y + 80 - regenClip.h;
            heartTexture.render(80 * i, 80 - regenClip.h, &regenClip);
        }
    }
    for (int i = 0; i < player->getKeys(); i++) {
        keyTexture.render(i * keyTexture.getWidth(), 80);
    }
    if (isCreatingNewTwinkle) {
        heartTwinkleFrames.push_back(0);
        heartTwinklePositions.push_back({heartTwinkleNum * 80 + (rand() % 31 + 20), rand() % 41 + 15});
    }
    for (int i = 0; i < (int)heartTwinkleFrames.size(); i++) {
        heartTwinkleTexture.render(heartTwinklePositions[i].x, heartTwinklePositions[i].y, &heartTwinkleClips[heartTwinkleFrames[i] / 10]);
        heartTwinkleFrames[i]++;
    }
    if (!heartTwinkleFrames.empty() && heartTwinkleFrames[0] == 60) {
        heartTwinkleFrames.erase(heartTwinkleFrames.begin());
        heartTwinklePositions.erase(heartTwinklePositions.begin());
    }
    isCreatingNewTwinkle = false;
    heartTwinkleNum = -1;
}
void gameClose()
{
    for (int i = 0; i < tileCount; i++) {
        if (tiles[i]) delete tiles[i];
    }
    tiles.clear();
    delete player;
    tileTexture.free();
    keyTexture.free();
    heartTexture.free();
    heartTwinkleTexture.free();
}