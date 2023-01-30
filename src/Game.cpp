#include "Game.h"

Resolution levelDimensions[LEVEL_TOTAL] = {
    {4000, 1080},
    {6000, 6000},
    {10000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080}
};
SDL_Point levelStartPositions[LEVEL_TOTAL] = {
    { 0, 1020},
    {80,  320},
    { 0, 1020},
    { 0, 1020},
    { 0, 1020},
    { 0, 1020},
    { 0, 1020},
    { 0, 1020},
    { 0, 1020},
    { 0, 1020}
};
int levelFinishTimes[LEVEL_TOTAL] = {80, 999, 999, 999, 999, 999, 999, 999, 999, 999};

SDL_Rect camera = {0, 0, LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT};

float timeStep;
Uint32 timeTicks;

int tileCount;
std::vector<CTile*> tiles;
SDL_Rect tileClips[TILE_TOTAL];
CTexture tileTexture;

CTexture keyTexture;
CTexture heartTexture;
SDL_Rect heartClips[4] = {{0, 0, 80, 80}, {0, 80, 80, 80}, {0, 160, 80, 80}, {0, 240, 80, 80}};
CTexture heartTwinkleTexture;
SDL_Rect heartTwinkleClips[6] = {{0, 0, 9, 9}, {9, 0, 9, 9}, {18, 0, 9, 9}, {0, 9, 9, 9}, {9, 9, 9, 9}, {18, 9, 9, 9}};
std::vector<int> heartTwinkleFrames;
std::vector<SDL_Point> heartTwinklePositions;

bool isDead;
CTexture gameOverTexture;

CTexture bgTexture;
CTexture bgPTexture;
float parallaxOffset;
std::string bgNames[5] = {"res/bgONE.png", "res/bgTWO.png", "res/bgTHREE.png", "res/bgFOUR.png", "res/bgFIVE.png"};
std::string bgParallaxNames[5] = {"res/bgONE_P.png", "res/bgTWO_P.png", "res/bgTHREE_P.png", "res/bgFOUR_P.png", "res/bgFIVE_P.png"};

SDL_Color gameButtonTextColour = {0xFF, 0xFF, 0xFF, 0xFF};
std::string gameButtonBackgroundColours[3] = {"#006F00", "#003F00", "#003F3F"};

CButton* gameButtons[GAME_BUTTON_TOTAL];

CPlayer* player;
std::vector<CProjectile*> projectiles;
CTexture projectileTexture;

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
        if (tileType == TILE_MEDAL && save.collectedMedals[save.level]) {
            tileType = TILE_EMPTY;
        }
        if(tileType >= 0 && tileType < TILE_TOTAL) {
            tiles.push_back(new CTile(x, y, tileType));
        } else {
            printf("Error loading map: Invalid tile type at %d\n", i);
            return false;
        }
        x += CTile::TILE_WIDTH;
        if(x >= levelDimensions[save.level - 1].w) {
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
    for (int i = 0; i < 5; i++) {
        if (save.collectedKeys[i] != -1) tiles[save.collectedKeys[i]]->setType(TILE_EMPTY);
        if (save.unlockedLocks[i] != -1) tiles[save.unlockedLocks[i]]->setType(TILE_EMPTY);
    }
    return true;
}

void setLevel(int level)
{
    setWindowIcon(level);
    bgTexture.loadFromFile(bgNames[(level - 1) / 2]);
    bgPTexture.loadFromFile(bgParallaxNames[(level - 1) / 2]);
    for (int i = 0; i < tileCount; i++) {
        if (tiles[i]) delete tiles[i];
    }
    tiles.clear();
    parallaxOffset = -1 * (rand() % LOGICAL_SCREEN_WIDTH);
    save.level = level;
    projectiles.clear();
    if (save.chapterTime <= levelFinishTimes[level - 1]) save.score += 100;
    save.chapterTime = 0;
    player->setPos(levelStartPositions[save.level - 1].x, levelStartPositions[save.level - 1].y);
    for (int i = 0; i < 5; i++) {
        save.collectedKeys[i] = -1;
        save.unlockedLocks[i] = -1;
    }
    if (save.level > maxLevel) {
        maxLevel = save.level;
        menuBackground.loadFromFile(bgNames[(maxLevel - 1) / 2]);
        savePersistent();
    }
    char* slotFile = (char*)calloc(20, sizeof(char));
    sprintf(slotFile, "saves/save_%s.bin", save.slot);
    SDL_RWops* writeFile = SDL_RWFromFile(slotFile, "wb");
    SDL_RWwrite(writeFile, &save, sizeof(Save), 1);
    SDL_RWclose(writeFile);
    timeTicks = SDL_GetTicks();
    tileCount = (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) * (levelDimensions[save.level - 1].h / CTile::TILE_HEIGHT);
    setTiles();
}

void gameDeathRetryCall()
{
    isDead = false;
}
void gameDeathQuitCall()
{
    isDead = false;
    transition(SCENE_MAINMENU);
}

bool gameLoadMedia()
{
    setWindowIcon(save.level);
    gameOverTexture.loadFromRenderedText("You Died!", gameButtonTextColour, "res/04b.TTF", 100);
    gameButtons[GAME_BUTTON_DEATHRETRY] = new CButton(0, 0, 80, gameButtonBackgroundColours, "Retry", gameButtonTextColour, &gameDeathRetryCall);
    gameButtons[GAME_BUTTON_DEATHRETRY]->setPos((LOGICAL_SCREEN_WIDTH - gameButtons[GAME_BUTTON_DEATHRETRY]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - gameButtons[GAME_BUTTON_DEATHRETRY]->getH()) / 2);
    gameButtons[GAME_BUTTON_DEATHQUIT] = new CButton(0, 0, 80, gameButtonBackgroundColours, "Quit", gameButtonTextColour, &gameDeathQuitCall);
    gameButtons[GAME_BUTTON_DEATHQUIT]->setPos((LOGICAL_SCREEN_WIDTH - gameButtons[GAME_BUTTON_DEATHQUIT]->getW()) / 2, gameButtons[GAME_BUTTON_DEATHRETRY]->getY() + 120);
    bgTexture.loadFromFile(bgNames[(save.level - 1) / 2]);
    bgPTexture.loadFromFile(bgParallaxNames[(save.level - 1) / 2]);
    parallaxOffset = -1 * (rand() % LOGICAL_SCREEN_WIDTH);
    keyTexture.loadFromFile("res/key.png");
    heartTexture.loadFromFile("res/hearts.png");
    heartTwinkleTexture.loadFromFile("res/heartTwinkle.png");
    timeTicks = SDL_GetTicks();
    player = new CPlayer(save.x, save.y);
    tileTexture.loadFromFile("res/tilesDEBUG.png");
    tileCount = (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) * (levelDimensions[save.level - 1].h / CTile::TILE_HEIGHT);
    setTiles();
    projectileTexture.loadFromFile("res/projectiles.png");
    for (int i = 0; i < (int)projectiles.size(); i++) {
        if (projectiles[i]->getEditTileIndex() != -1) tiles[projectiles[i]->getEditTileIndex()]->setType(projectiles[i]->getEditTileOriginal());
    }
    projectiles.push_back(new CProjectile(400, 300, PROJECTILE_DAMAGEBALL, 300, 0));
    projectiles.push_back(new CProjectile(640, 300, PROJECTILE_DAMAGEBALL, 300, 0, 400));
    projectiles.push_back(new CProjectile(80, 1015, 2500, TILE_WATERUP, false));
    projectiles.push_back(new CProjectile(400, 500, PROJECTILE_SHIELD, 0, 0));
    projectiles.push_back(new CProjectile(320, 760, 40, 240, 440, 760, "Junaid is so cute!!!!", SDL_Color{0xFF, 0xFF, 0xFF}, 40));
    return true;
}
void gameHandleEvent(SDL_Event* e)
{
    player->handleEvent(e);
    if (isDead) {
        for (int i = 0; i < GAME_BUTTON_TOTAL; i++) {
            if (gameButtons[i]) gameButtons[i]->handleEvent(e);
        }
        return;
    }
    if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_h) {
        projectiles.push_back(new CProjectile(player->getPosX(), player->getPosY() - 50, PROJECTILE_HEART, 0, 0));
    } else if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_j) {
        projectiles.push_back(new CProjectile(player->getPosX(), player->getPosY() - 50, PROJECTILE_SHIELD, 0, 0));
    } else if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_k) {
        projectiles.push_back(new CProjectile(player->getPosX(), player->getPosY() - 50, PROJECTILE_CHARGER, 0, 0));
    }
    if ((e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_ESCAPE) || (e->type == SDL_JOYBUTTONUP && e->jbutton.button == SDL_CONTROLLER_BUTTON_START)) {
        transition(SCENE_PAUSE);
    }
}
void gameUpdate()
{
    if (rand() % 20 == 0 && save.difficulty == DIFFICULTY_EASY && player->getHealth() < save.maxHealth) {
        heartTwinkleFrames.push_back(0);
        heartTwinklePositions.push_back({player->getHealth() * 80 + (rand() % 31 + 20), rand() % 41 + 15});
    } else {
        for (int i = 0; i < (int)heartTwinkleFrames.size(); i++) {
            heartTwinkleFrames[i]++;
        }
    }
    if (!heartTwinkleFrames.empty() && heartTwinkleFrames[0] >= 60) {
        heartTwinkleFrames.erase(heartTwinkleFrames.begin());
        heartTwinklePositions.erase(heartTwinklePositions.begin());
    }
    timeStep = (SDL_GetTicks() - timeTicks) / 1000.f;
    if (isDead) {
        timeTicks = SDL_GetTicks();
        return;
    }
    player->move(timeStep);
    for (int i = 0; i < (int)projectiles.size(); i++) {
        if (projectiles[i]) projectiles[i]->move(timeStep);
    }
    for (int i = 0; i < tileCount; i++) {
        tiles[i]->updateTiles(timeStep);
    }
    save.chapterTime += timeStep;
    save.totalTime += timeStep;
    timeTicks = SDL_GetTicks();
    player->setCamera(camera);
    player->checkItemCollisions();
    if (parallaxOffset < -LOGICAL_SCREEN_WIDTH) parallaxOffset += LOGICAL_SCREEN_WIDTH;
    if (parallaxOffset > 0) parallaxOffset -= LOGICAL_SCREEN_WIDTH;
}
void gameRender()
{
    SDL_SetRenderDrawColor(gRenderer, 69, 69, 69, 0xFF);
    SDL_RenderClear(gRenderer);
    bgTexture.render(0, 0);
    bgPTexture.render((int)parallaxOffset, 0);
    bgPTexture.render((int)parallaxOffset + LOGICAL_SCREEN_WIDTH, 0);
    for (int i = 0; i < tileCount; i++) {
        tiles[i]->render(camera);
    }
    player->render(camera);
    for (int i = 0; i < (int)projectiles.size(); i++) {
        if (projectiles[i]) projectiles[i]->render(camera);
    }
    for (int i = 0; i < save.maxHealth; i++) {
        heartTexture.render(80 * i, 0, &heartClips[0]);
        if (i < player->getHealth() && save.difficulty != DIFFICULTY_EASY) heartTexture.render(80 * i, 0, &heartClips[1]);
        if (i < player->getHealth() && save.difficulty == DIFFICULTY_EASY) heartTexture.render(80 * i, 0, &heartClips[2]);
        if (i == player->getHealth() && save.difficulty == DIFFICULTY_EASY) {
            SDL_Rect regenClip = heartClips[2];
            regenClip.h = (int)(safePositionTimeTimerSeconds * heartClips[2].h / safePositionTimeSeconds);
            regenClip.y = heartClips[2].y + 80 - regenClip.h;
            heartTexture.render(80 * i, 80 - regenClip.h, &regenClip);
        }
    }
    for (int i = 0; i < player->getShield(); i++) {
        heartTexture.render(80 * (save.maxHealth + i), 0, &heartClips[3]);
    }
    if (!heartTwinklePositions.empty()) for (int i = 0; i < (int)heartTwinkleFrames.size(); i++) {
        heartTwinkleTexture.render(heartTwinklePositions[i].x, heartTwinklePositions[i].y, &heartTwinkleClips[heartTwinkleFrames[i] / 10]);
    }
    for (int i = 0; i < player->getKeys(); i++) {
        keyTexture.render(i * keyTexture.getWidth(), 80);
    }
    if (isDead) {
        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xAF);
        SDL_RenderFillRect(gRenderer, NULL);
        gameOverTexture.render((LOGICAL_SCREEN_WIDTH - gameOverTexture.getWidth()) / 2, (LOGICAL_SCREEN_HEIGHT - gameOverTexture.getHeight()) / 4);
        for (int i = 0; i < GAME_BUTTON_TOTAL; i++) {   
            if (gameButtons[i]) gameButtons[i]->render();
        }
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
    player = NULL;
    for (int i = 0; i < (int)projectiles.size(); i++) {
        delete projectiles[i];
    }
    projectiles.clear();
    tileTexture.free();
    keyTexture.free();
    heartTexture.free();
    heartTwinkleTexture.free();
    heartTwinkleFrames.clear();
    heartTwinklePositions.clear();
    projectileTexture.free();
    gameOverTexture.free();
}