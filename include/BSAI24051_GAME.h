#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "BSAI24051_PLAYER.h"
#include "BSAI24051_ENEMY.h"
#include "BSAI24051_HELICOPTER.h"
#include "BSAI24051_PACK.h"
#include "BSAI24051_KNIFETHROWER.h"
#include "BSAI24051_BIGAIRSHIP.h"
#include "CONSTANTS.h"

class Game {
public:
    bool mainmenu;
    bool isPaused;
    Player player;
    Enemy enemies[maxenemies];
    Helicopter helicopters[maxhelicopters];
    int score;
    int level;
    Texture2D background;
    Texture2D background2;
    Texture2D levelTwoBackground;
    Texture2D enemyTexture;
    Texture2D helicopterTexture;
    int bg2x;
    int bgx;
    Pack healthPack;
    Pack ammoPack;
    KnifeThrower knifeThrowers[maxKnifeThrowers];
    Texture2D levelThreeBackground;
    BigAirship bigAirship;
    bool levelFourStarted;

    Game();
    void initEnemies();
    void initHelicopters();
    void initKnifeThrowers();
    void loadTextures();
    void spawnEnemy();
    void spawnHelicopter();
    void spawnKnifeThrowers();
    void update();
    void draw();
    void drawMainMenu();
    void drawGameOver();
    void restart();
    void pauseGame();
    void run();
};

#endif
