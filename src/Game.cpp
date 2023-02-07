#include "Game.h"

Resolution levelDimensions[LEVEL_TOTAL] = {
    {4000, 1080},
    {6000, 4000},
    {8000, 2000},
    {5000, 5000},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080},
    {4000, 1080}
};
SDL_Point levelStartPositions[LEVEL_TOTAL] = {
    {  0, 1020},
    { 80,  320},
    {490, 1540},
    { 70, 260},
    {  0, 1020},
    {  0, 1020},
    {  0, 1020},
    {  0, 1020},
    {  0, 1020},
    {  0, 1020}
};
int levelFinishTimes[LEVEL_TOTAL] = {80, 145, 230, 999, 999, 999, 999, 999, 999, 999};

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

bool isEndLevel;
CTexture levelCompleteTexture, scoreTexture;

CTexture bgTexture;
CTexture bgPTexture;
float parallaxOffset;
std::string bgNames[5] = {"res/bgONE.png", "res/bgTWO.png", "res/bgTHREE.png", "res/bgFOUR.png", "res/bgFIVE.png"};
std::string bgParallaxNames[5] = {"res/bgONE_P.png", "res/bgTWO_P.png", "res/bgTHREE_P.png", "res/bgFOUR_P.png", "res/bgFIVE_P.png"};

CButton* gameButtons[GAME_BUTTON_TOTAL];

CPlayer* player;
std::vector<CProjectile*> projectiles;
CTexture projectileTexture;

CTexture rainbowIndicatorTexture;
Uint8 rainbowIndicatorRGB[3] = {0xFF, 0x00, 0x00};

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
    if(a.y + a.h <= b.y) return false;
    if(b.y + b.h <= a.y) return false;
    if(a.x + a.w <= b.x) return false;
    if(b.x + b.w <= a.x) return false;
    return true;
}
void setProjectiles()
{
    switch (save.level) {
        case 1: {
            const char* tutorialKeybindsText[5] = {"Up", "Left", "Down", "Right", "Jump"};
            char* curText = (char*)calloc(50, sizeof(char));
            for (int i = 0; i < KEYBINDS_TOTAL; i++) {
                sprintf(curText, "%s - %s", tutorialKeybindsText[i], SDL_GetKeyName(keybinds[i]));
                projectiles.push_back(new CProjectile(0, 1020, 20, 20, 20, 810 + 25 * i, curText, SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            }
            sprintf(curText, "Hold %s longer", SDL_GetKeyName(keybinds[KEYBINDS_JUMP]));
            projectiles.push_back(new CProjectile(320, 760, 40, 240, 400, 820, curText, SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            projectiles.push_back(new CProjectile(320, 760, 40, 240, 400, 845, "to jump higher!", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            projectiles.push_back(new CProjectile(960, 800, 200, 40, 1210, 770, "< Requires a key", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            projectiles.push_back(new CProjectile(760, 600, 40, 40, 480, 420, "White: Affects Ghost Blocks", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            projectiles.push_back(new CProjectile(1480, 1000, 40, 40, 1240, 580, "Green: Jump 50% Higher", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            projectiles.push_back(new CProjectile(1920, 720, 40, 40, 1900, 540, "Red: Move 50% Quicker", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            projectiles.push_back(new CProjectile(2600, 680, 40, 40, 2820, 340, "Blue: Stick To and Climb Walls", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            projectiles.push_back(new CProjectile(2600, 680, 40, 40, 2820, 365, "Use Up and Down on a wall!", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            projectiles.push_back(new CProjectile(3760, 720, 40, 40, 3310, 730, "Restores Your Jumps >", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            projectiles.push_back(new CProjectile(160, 0, 40, 40, 930, 85, "Gold Medals give extra score", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            projectiles.push_back(new CProjectile(2760, 0, 40, 40, 2820, 520, "Collect the Purple Medal", SDL_Color{0xFF, 0xFF, 0xFF}, 40));
            projectiles.push_back(new CProjectile(2760, 0, 40, 40, 2820, 560, "to complete levels!", SDL_Color{0xFF, 0xFF, 0xFF}, 40));
            projectiles.push_back(new CProjectile(1840, 440, 40, 40, 2080, 850, "Spikes are dangerous...", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            break;
        }
        case 2: {
            if ((int)projectiles.size() == 0) {
                projectiles.push_back(new CProjectile(4295, 855, PROJECTILE_SHIELD, 0, 0)); //Shield intended to let hard difficulty players access the secret gold medal
                //Tile changing buttons
                projectiles.push_back(new CProjectile(5370, 874, 2234, TILE_WHITECRYSTAL, true)); //Activates crystal for gold medal
                projectiles.push_back(new CProjectile(640, 3434, 13389, TILE_EMPTY, false)); //Lower left side, lets white jump over a grass pillar
                projectiles.push_back(new CProjectile(3967, 3474, 13597, TILE_GRASS_TOPMIDDLE, false)); //Lower right side, places block so blue can get up for next button
                projectiles.push_back(new CProjectile(4128, 3114, 12402, TILE_GRASS_MIDDLE, false)); //Lower right side, places block so red can get up for a key
            }
            projectiles.push_back(new CProjectile(487, 1927, 2807, 3855)); //Teleports player from the end of the middle left puzzle to below the exit to the next level
            projectiles.push_back(new CProjectile(1727, 2807, 800, 1560)); //Teleports player from lower left side to the middle left puzzle
            projectiles.push_back(new CProjectile(1247, 3767, 5920, 1500)); //Teleports player from lower left side to middle right section
            projectiles.push_back(new CProjectile(5967, 1567, 2807, 3855)); //Teleports player from middle right section to below the exit to the next level
            projectiles.push_back(new CProjectile(5847, 1127, 760, 3300)); //Teleports player from middle right section to the lower left button 
            //Text spawners
            projectiles.push_back(new CProjectile(4295, 855, 25, 25, 4160, 610, "Shields serve as an extra life", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            projectiles.push_back(new CProjectile(4800, 40, 40, 840, 5150, 725, "Don't try to swim!", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            //Toggle buttons for the middle left key puzzle
            projectiles.push_back(new CProjectile(600, 1754, 6612, TILE_GRASS_MIDDLE, TILE_EMPTY));
            projectiles.push_back(new CProjectile(200, 1754, 6762, TILE_GRASS_MIDDLE, TILE_EMPTY));
            projectiles.push_back(new CProjectile(200, 1754, 6912, TILE_GRASS_MIDDLE, TILE_EMPTY));
            projectiles.push_back(new CProjectile(375, 1754, 6612, TILE_GRASS_MIDDLE, TILE_EMPTY));
            projectiles.push_back(new CProjectile(375, 1754, 6912, TILE_GRASS_MIDDLE, TILE_EMPTY));
            break;
        }
        case 3: {
            if ((int)projectiles.size() == 0) {
                //Extra lives to aid the player
                projectiles.push_back(new CProjectile(2127, 55, PROJECTILE_HEART, 0, 0));
                projectiles.push_back(new CProjectile(2448, 55, PROJECTILE_HEART, 0, 0));
                projectiles.push_back(new CProjectile(6607, 1215, PROJECTILE_HEART, 0, 0));
                //Absorbs damange for the player
                projectiles.push_back(new CProjectile(5328, 1215, PROJECTILE_SHIELD, 0, 0));
                //Charges the player's rainbow ability at the end of the map
                for (int i = 0; i < 20; i++) {
                    projectiles.push_back(new CProjectile(6687 + i % 2, 327 + 80 * i, PROJECTILE_CHARGER, 0, 0, 6680, 327 + 80 * i));
                }
                //Tile changing buttons
                projectiles.push_back(new CProjectile(687, 1834, 7800, TILE_BOUNCE, true)); //Place a bounce block for the secret gold medal
                projectiles.push_back(new CProjectile(7007, 1954, 4800, TILE_JUMPCRYSTAL, true)); //Activates a jump crystal for the secret gold medal
                projectiles.push_back(new CProjectile(4407, 314, 2800, TILE_JUMPCRYSTAL, true)); //Activates a jump crystal for the secret gold medal
                projectiles.push_back(new CProjectile(5127, 1354, 1400, TILE_JUMPCRYSTAL, true)); //Activates a jump crystal for the secret gold medal
                projectiles.push_back(new CProjectile(5327, 594, 1551, TILE_STONE, true)); //Places a block to access the final key
                projectiles.push_back(new CProjectile(2887, 1354, 8070, TILE_BOUNCE, false)); //Places a block to access the top lake key
                projectiles.push_back(new CProjectile(3967, 394, 8083, TILE_REDCRYSTAL, true)); //Removes a block to continue to the final building
                projectiles.push_back(new CProjectile(6607, 1154, 6365, TILE_EMPTY, true)); //Removes a block to drop a heart
                projectiles.push_back(new CProjectile(6487, 1394, 6333, TILE_EMPTY, true)); //Removes a block to drop a shield
            }
            projectiles.push_back(new CProjectile(5327, 967, 6250, 1220)); //Teleports the player from the final key room to outside of it in case they are trapped
            projectiles.push_back(new CProjectile(6615, 360, 7170, 1940)); //Teleports the player from the final key room to a button for the secret gold medal
            projectiles.push_back(new CProjectile(6847, 1927, 6330, 1820)); //Teleports the player from the previous button room to outside
            projectiles.push_back(new CProjectile(3007, 767, 770, 1820)); //Teleports the player from near the top lake area to a button for the secret gold medal
            projectiles.push_back(new CProjectile(607, 1807, 610, 1540)); //Teleports the player from the previous button room to outside
            projectiles.push_back(new CProjectile(3287, 1327, 3250, 300)); //Teleports the player from the lower part of the map to the lake key part
            projectiles.push_back(new CProjectile(2207, 1567, 2010, 900)); //Teleports the player from the lower part of the map to the higher part
            //Toggle buttons for the final key puzzle
            projectiles.push_back(new CProjectile(5447, 1154, 5757, TILE_JUNGLE_MIDDLE, TILE_EMPTY));
            projectiles.push_back(new CProjectile(5447, 1154, 5759, TILE_JUNGLE_MIDDLE, TILE_EMPTY));
            projectiles.push_back(new CProjectile(5447, 1154, 5760, TILE_EMPTY, TILE_JUNGLE_MIDDLE));
            projectiles.push_back(new CProjectile(5607, 1154, 5758, TILE_JUNGLE_MIDDLE, TILE_EMPTY));
            projectiles.push_back(new CProjectile(5607, 1154, 5759, TILE_JUNGLE_MIDDLE, TILE_EMPTY));
            projectiles.push_back(new CProjectile(5607, 1154, 5761, TILE_JUNGLE_MIDDLE, TILE_EMPTY));
            projectiles.push_back(new CProjectile(5767, 1154, 5757, TILE_JUNGLE_MIDDLE, TILE_EMPTY));
            projectiles.push_back(new CProjectile(5767, 1154, 5759, TILE_JUNGLE_MIDDLE, TILE_EMPTY));
            projectiles.push_back(new CProjectile(5767, 1154, 5761, TILE_JUNGLE_MIDDLE, TILE_EMPTY));
            projectiles.push_back(new CProjectile(5927, 1154, 5758, TILE_JUNGLE_MIDDLE, TILE_EMPTY));
            projectiles.push_back(new CProjectile(5927, 1154, 5760, TILE_EMPTY, TILE_JUNGLE_MIDDLE));
            projectiles.push_back(new CProjectile(6087, 1154, 5758, TILE_JUNGLE_MIDDLE, TILE_EMPTY));
            projectiles.push_back(new CProjectile(6087, 1154, 5759, TILE_JUNGLE_MIDDLE, TILE_EMPTY));
            //Text spawners
            projectiles.push_back(new CProjectile(840, 1559, 40, 1, 240, 1120, "Bounce Blocks allow you", SDL_Color{0x00, 0xFF, 0x00}, 25));
            projectiles.push_back(new CProjectile(840, 1559, 40, 1, 240, 1150, "to jump twice as high!", SDL_Color{0x00, 0xFF, 0x00}, 25));
            projectiles.push_back(new CProjectile(6680, 280, 40, 40, 6330, 127, "Collect 20 Rainbow Charges", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            projectiles.push_back(new CProjectile(6680, 280, 40, 40, 6330, 167, "and press Q to gain flight", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            projectiles.push_back(new CProjectile(6680, 280, 40, 40, 6330, 207, "and invulnerability for a short time", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            projectiles.push_back(new CProjectile(6760, 80, 40, 240, 6850, 447, "Fly down here!", SDL_Color{0xFF, 0xFF, 0xFF}, 25));
            //Bounce blocks based around the level
            projectiles.push_back(new CProjectile(840, 1559, 40, 1, PROJECTILE_BOUNCEBLOCK));
            projectiles.push_back(new CProjectile(0, 1559, 40, 1, PROJECTILE_BOUNCEBLOCK));
            projectiles.push_back(new CProjectile(1800, 919, 40, 1, PROJECTILE_BOUNCEBLOCK));
            projectiles.push_back(new CProjectile(1840, 559, 40, 1, PROJECTILE_BOUNCEBLOCK));
            projectiles.push_back(new CProjectile(2760, 1839, 40, 1, PROJECTILE_BOUNCEBLOCK));
            projectiles.push_back(new CProjectile(2800, 1599, 40, 1, PROJECTILE_BOUNCEBLOCK));
            break;
        }
        case 4: {
            projectiles.push_back(new CProjectile(2600, 559, 40, 1, PROJECTILE_BOUNCEBLOCK));
            break;
        }
    }
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
    if (player->getForm() == FORM_RAINBOW) player->setForm(FORM_WHITE);
    bgTexture.loadFromFile(bgNames[(level - 1) / 2]);
    bgPTexture.loadFromFile(bgParallaxNames[(level - 1) / 2]);
    for (int i = 0; i < tileCount; i++) {
        if (tiles[i]) delete tiles[i];
    }
    tiles.clear();
    parallaxOffset = -1 * (rand() % LOGICAL_SCREEN_WIDTH);
    if (level > save.level || isDead) {
        SDL_ShowCursor(SDL_ENABLE);
        for (int i = 0; i < (int)projectiles.size(); i++) {
            delete projectiles[i];
        }
        projectiles.clear();
    }
    if (level > save.level) {
        isEndLevel = true;
        if (save.chapterTime <= levelFinishTimes[level - 1]) save.score += 100;
        save.chapterTime = 0;
        scoreTexture.loadFromRenderedText("Score: " + std::to_string(save.score), SDL_Color{0xFF, 0xFF, 0xFF}, 80);
    }
    save.level = level;
    player->setPos(levelStartPositions[save.level - 1].x, levelStartPositions[save.level - 1].y);
    player->setCamera(camera);
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
    setProjectiles();
}

void gameDeathRetryCall()
{
    SDL_ShowCursor(SDL_DISABLE);
    isDead = false;
}
void gameDeathQuitCall()
{
    isDead = false;
    transition(SCENE_MAINMENU);
}
void gameContinueCall()
{
    SDL_ShowCursor(SDL_DISABLE);
    isEndLevel = false;
}

bool gameLoadMedia()
{
    SDL_ShowCursor(SDL_DISABLE);
    setWindowIcon(save.level);
    gameOverTexture.loadFromRenderedText("You Died!", SDL_Color{0xFF, 0xFF, 0xFF}, 100);
    levelCompleteTexture.loadFromRenderedText("Level Complete!", SDL_Color{0xFF, 0xFF, 0xFF}, 100);
    gameButtons[GAME_BUTTON_DEATHRETRY] = new CButton(0, 0, 80, "Retry", &gameDeathRetryCall);
    gameButtons[GAME_BUTTON_DEATHRETRY]->setPos((LOGICAL_SCREEN_WIDTH - gameButtons[GAME_BUTTON_DEATHRETRY]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - gameButtons[GAME_BUTTON_DEATHRETRY]->getH()) / 2);
    gameButtons[GAME_BUTTON_DEATHQUIT] = new CButton(0, 0, 80, "Quit", &gameDeathQuitCall);
    gameButtons[GAME_BUTTON_DEATHQUIT]->setPos((LOGICAL_SCREEN_WIDTH - gameButtons[GAME_BUTTON_DEATHQUIT]->getW()) / 2, gameButtons[GAME_BUTTON_DEATHRETRY]->getY() + 120);
    gameButtons[GAME_BUTTON_CONTINUE] = new CButton(0, 0, 80, "Continue", &gameContinueCall);
    gameButtons[GAME_BUTTON_CONTINUE]->setPos((LOGICAL_SCREEN_WIDTH - gameButtons[GAME_BUTTON_CONTINUE]->getW()) / 2, (LOGICAL_SCREEN_HEIGHT - gameButtons[GAME_BUTTON_CONTINUE]->getH()) / 2);
    bgTexture.loadFromFile(bgNames[(save.level - 1) / 2]);
    bgPTexture.loadFromFile(bgParallaxNames[(save.level - 1) / 2]);
    parallaxOffset = -1 * (rand() % LOGICAL_SCREEN_WIDTH);
    keyTexture.loadFromFile("res/key.png");
    heartTexture.loadFromFile("res/hearts.png");
    heartTwinkleTexture.loadFromFile("res/heartTwinkle.png");
    rainbowIndicatorTexture.loadFromFile("res/rainbowIndicator.png");
    timeTicks = SDL_GetTicks();
    player = new CPlayer(save.x, save.y);
    tileTexture.loadFromFile("res/tilesDEBUG.png");
    tileCount = (levelDimensions[save.level - 1].w / CTile::TILE_WIDTH) * (levelDimensions[save.level - 1].h / CTile::TILE_HEIGHT);
    setTiles();
    projectileTexture.loadFromFile("res/projectiles.png");
    for (int i = 0; i < (int)projectiles.size(); i++) {
        if (projectiles[i]->getType() == PROJECTILE_BUTTON_TILECHANGE) tiles[projectiles[i]->getEditTileIndex()]->setType(projectiles[i]->getEditTileOriginal());
    }
    setProjectiles();
    // projectiles.push_back(new CProjectile(400, 300, PROJECTILE_DAMAGEBALL, 300, 0));
    // projectiles.push_back(new CProjectile(640, 300, PROJECTILE_DAMAGEBALL, 300, 0, 400));
    return true;
}
void gameHandleEvent(SDL_Event* e)
{
    player->handleEvent(e);
    if (isDead) {
        gameButtons[GAME_BUTTON_DEATHRETRY]->handleEvent(e);
        gameButtons[GAME_BUTTON_DEATHQUIT]->handleEvent(e);
        return;
    } else if (isEndLevel) {
        gameButtons[GAME_BUTTON_CONTINUE]->handleEvent(e);
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
        SDL_ShowCursor(SDL_ENABLE);
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
    if (isDead || isEndLevel) {
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
    player->checkSpecialTileCollisions();
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
    for (int i = 0; i < (int)projectiles.size(); i++) {
        if (projectiles[i]) projectiles[i]->render(camera);
    }
    player->render(camera);
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
    if (player->getCharge() >= CPlayer::MAX_CHARGE || player->getForm() == FORM_RAINBOW) {
        rainbowIndicatorTexture.render(0, 0);
        if(rainbowIndicatorRGB[2] == 0xFF) {
            rainbowIndicatorRGB[1] == 0 ? rainbowIndicatorRGB[0]++ : rainbowIndicatorRGB[1]--;
            if (rainbowIndicatorRGB[0] == 0xFF) rainbowIndicatorRGB[2]--;
        }
        else if(rainbowIndicatorRGB[1] == 0xFF) rainbowIndicatorRGB[0] == 0 ? rainbowIndicatorRGB[2]++ : rainbowIndicatorRGB[0]--;
        else if(rainbowIndicatorRGB[0] == 0xFF) rainbowIndicatorRGB[2] == 0 ? rainbowIndicatorRGB[1]++ : rainbowIndicatorRGB[2]--;
        rainbowIndicatorTexture.setColour(rainbowIndicatorRGB[0], rainbowIndicatorRGB[1], rainbowIndicatorRGB[2]);
    }
    if (isDead) {
        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xAF);
        SDL_RenderFillRect(gRenderer, NULL);
        gameOverTexture.render((LOGICAL_SCREEN_WIDTH - gameOverTexture.getWidth()) / 2, (LOGICAL_SCREEN_HEIGHT - gameOverTexture.getHeight()) / 4);
        gameButtons[GAME_BUTTON_DEATHRETRY]->render();
        gameButtons[GAME_BUTTON_DEATHQUIT]->render();
    } else if (isEndLevel) {
        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xAF);
        SDL_RenderFillRect(gRenderer, NULL);
        levelCompleteTexture.render((LOGICAL_SCREEN_WIDTH - levelCompleteTexture.getWidth()) / 2, (LOGICAL_SCREEN_HEIGHT - levelCompleteTexture.getHeight()) / 4);
        scoreTexture.render(10, LOGICAL_SCREEN_HEIGHT - scoreTexture.getHeight() - 10);
        gameButtons[GAME_BUTTON_CONTINUE]->render();
    }
}
void gameClose()
{
    SDL_ShowCursor(SDL_ENABLE);
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
    levelCompleteTexture.free();
    scoreTexture.free();
}