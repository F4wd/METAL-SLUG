#include "BSAI24051_GAME.h"


Game::Game() {
    mainmenu = true;
    isPaused = false;
    score = 0;
    level = 1;
    bg2x = 4 * screenwidth;
    bgx = 0;
    levelFourStarted = false;
    gameCompleted = false;
    initEnemies();
    initHelicopters();
    initKnifeThrowers();
}

void Game::initEnemies() {
    for (int i = 0; i < maxenemies; i++) {
        enemies[i] = Enemy();
    }
}

void Game::initHelicopters() {
    for (int i = 0; i < maxhelicopters; i++) {
        helicopters[i] = Helicopter();
    }
}

void Game::initKnifeThrowers() {
    for (int i = 0; i < maxKnifeThrowers; i++) {
        knifeThrowers[i] = KnifeThrower();
    }
}

void Game::loadTextures() {
    background2 = LoadTexture("Map2.png");
    background2.height = screenheight;
    background2.width = 4 * screenwidth;

    background = LoadTexture("Map1.png");
    background.height = screenheight;
    background.width = 4 * screenwidth;

    levelTwoBackground = LoadTexture("bk4.png");
    levelTwoBackground.height = screenheight;
    levelTwoBackground.width = 4 * screenwidth;

    enemyTexture = LoadTexture("EnemyTank.png");
    enemyTexture.height = screenheight / 4 - 150;
    enemyTexture.width = screenwidth / 4 - 150;

    levelThreeBackground = LoadTexture("heavy.png");
    levelThreeBackground.height = screenheight;
    levelThreeBackground.width = 4 * screenwidth;

    helicopterTexture = LoadTexture("myFeiji.png");
    helicopterTexture.height = 60;
    helicopterTexture.width = 80;

    player.loadPlayerImages();
    healthPack.loadTextures("pack.png", "tong.png");
    ammoPack.loadTextures("pack.png", "tong.png");

    bigAirship.loadTexture("big_airship.png");

    for (int i = 0; i < maxKnifeThrowers; i++) {
        knifeThrowers[i].loadTexture("hamzaTA.png");
    }
}

void Game::spawnEnemy() {
    int activeEnemies = 0;
    for (int i = 0; i < maxenemies; i++) {
        if (enemies[i].active)
            activeEnemies++;
    }

    int maxSpawn = (level == 1) ? 2 : 0; // Fewer tanks in level 2
    if (activeEnemies < maxSpawn) {
        for (int i = 0; i < maxenemies; i++) {
            if (!enemies[i].active) {
                enemies[i].spawn();
                break;
            }
        }
    }
}

void Game::spawnHelicopter() {
    if (level < 2 || level > 2) return; // Only allow in Level 2

    int activeHelicopters = 0;
    for (int i = 0; i < maxhelicopters; i++) {
        if (helicopters[i].active)
            activeHelicopters++;
    }

    if (activeHelicopters < 2 && GetRandomValue(0, 100) < 3) {
        for (int i = 0; i < maxhelicopters; i++) {
            if (!helicopters[i].active) {
                helicopters[i].spawn();
                break;
            }
        }
    }
}

void Game::spawnKnifeThrowers() {
    if (level < 3 || level > 3) return;

    int activeCount = 0;
    for (int i = 0; i < maxKnifeThrowers; i++) {
        if (knifeThrowers[i].active) activeCount++;
    }

    if (activeCount < 2 && GetRandomValue(0, 100) < 3) {
        for (int i = 0; i < maxKnifeThrowers; i++) {
            if (!knifeThrowers[i].active) {
                knifeThrowers[i].spawn();
                break;
            }
        }
    }
}

void Game::update() {
    if (player.health <= 0) {
        drawGameOver();
        return;
    }

    if (mainmenu) {
        drawMainMenu();
        return;
    }

    if (gameCompleted) {
        drawVictoryScreen();
        return;
    }

    float deltaTime = GetFrameTime();

    if (player.PLayerCoordinate.x >= screenwidth - 100) {
        player.PLayerCoordinate.x = screenwidth / 2 - 550;
    }

    if (bgx <= -background.width) {
        bgx = background.width - 5;
    }

    if (bg2x <= -background2.width) {
        bg2x = background2.width - 5;
    }

    // Level progression
    if (score >= 10 && level == 1) {
        level = 2;
        DrawText("LEVEL 2 - AIR ASSAULT!", screenwidth / 2 - 200,
            screenheight / 2, 40, YELLOW);
    }
    else if (score >= 18 && level == 2) {
        level = 3;
        DrawText("LEVEL 3 - HAMZA BHAI SE BACHO!", screenwidth / 2 - 250,
            screenheight / 2, 40, RED);
    }
    else if (score >= 30 && level == 3) {
        level = 4;
        levelFourStarted = false;
    }

    if (level == 4 && !levelFourStarted) {
        bigAirship.spawn();
        levelFourStarted = true;
    }
    if (level == 4) 
    {
        bigAirship.update(player, score);
        if (!bigAirship.active) 
        {
            gameCompleted = true;
            return;
        }
    }

    player.updateMovement(bgx, bg2x);
    player.updateAnimation(deltaTime);

    if (IsKeyPressed(KEY_S)) {
        player.shoot();
    }

    player.updateBullets();

    for (int i = 0; i < maxenemies; i++) {
        enemies[i].update(player, score);
    }

    for (int i = 0; i < maxhelicopters; i++) {
        helicopters[i].update(player, score);
    }
    for (int i = 0; i < maxKnifeThrowers; i++) {
        knifeThrowers[i].update(player, score);
    }

    spawnEnemy();
    spawnHelicopter();
    spawnKnifeThrowers();
    draw();

    if (IsKeyPressed(KEY_P)) {
        pauseGame();
    }
    healthPack.update(player);
    ammoPack.update(player);
}

void Game::draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (level == 1) {
        DrawTexture(background2, bgx, 0, WHITE);
        DrawTexture(background, bg2x, 0, WHITE);
    }
    else if (level == 2) {
        DrawTexture(levelTwoBackground, bgx, 0, WHITE);
        DrawTexture(background2, bg2x, 0, WHITE);
    }
    else if (level == 3) {
        DrawTexture(levelThreeBackground, bgx, 0, WHITE);
        DrawTexture(background2, bg2x, 0, WHITE);
    }
    else {
        bigAirship.draw();
        DrawText("Level 4 - Airship Assault!", 10, 200, 30, PURPLE);
    }

    player.draw();
    player.drawBullets();

    if (player.IsPLayerMoving) {
        if (player.IsPLayerMovingRIGHT) {
            bgx -= 6;
            bg2x -= 6;
        }
        else if (player.IsPLayerMovingLEFT) {
            bgx += 6;
            bg2x += 6;
        }
    }

    for (int i = 0; i < maxenemies; i++) {
        enemies[i].draw(enemyTexture);
        enemies[i].drawBullets();
    }

    for (int i = 0; i < maxhelicopters; i++) {
        helicopters[i].draw(helicopterTexture);
        helicopters[i].drawBullets();
    }
    for (int i = 0; i < maxKnifeThrowers; i++) {
        knifeThrowers[i].draw();
    }

    DrawText(TextFormat("Enemies Destroyed: %d", score), 10, 0, 40, RED);
    DrawText(TextFormat("Health: %d", player.health), 10, 50, 40, YELLOW);
    DrawText(TextFormat("Level: %d", level), 10, 100, 40, BLUE);

    if (level == 2) {
        DrawText("Air Assault Mode!", 10, 150, 30, ORANGE);
    }
    else if (level == 3) {
        DrawText("Knife Throwers!", 10, 150, 30, RED);
    }

    healthPack.draw();
    ammoPack.draw();

    EndDrawing();
}

void Game::drawMainMenu() {
    BeginDrawing();
    ClearBackground(BLACK);

    const char* title = "METAL SLUG";
    const char* subtitle = "PRESS ENTER TO START";

    int titleFontSize = 80;
    int titleWidth = MeasureText(title, titleFontSize);
    DrawText(title, screenwidth / 2 - titleWidth / 2,
        screenheight / 2 - titleFontSize, titleFontSize, RED);

    int subtitleFontSize = 40;
    int subtitleWidth = MeasureText(subtitle, subtitleFontSize);
    DrawText(subtitle, screenwidth / 2 - subtitleWidth / 2,
       screenheight/ 2 + titleFontSize / 2, subtitleFontSize, WHITE);

    if (IsKeyPressed(KEY_ENTER)) {
        mainmenu = false;
    }

    EndDrawing();
}

void Game::drawGameOver() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("GAME OVER", screenwidth/ 2 - 200,screenheight / 2 - 20, 70, RED);
    DrawText("Press R to Restart", screenwidth / 2 - 185, screenheight / 2 + 60, 40, GREEN);

    if (IsKeyPressed(KEY_R)) {
        restart();
    }

    EndDrawing();
}

void Game::restart() {
    player.health = 100;
    score = 0;
    level = 1;
    player.PLayerCoordinate.x = screenwidth/ 2 - 550;
    player.PLayerCoordinate.y = screenheight / 2 + 350;
    initEnemies();
    initHelicopters();
}

void Game::pauseGame() {
    isPaused = true;
    int countdown = 5;

    while (isPaused && countdown >= 0) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(TextFormat("Time Remaining: %d", countdown),
            screenheight / 2 - 150, screenwidth / 2 - 150, 50, WHITE);
        EndDrawing();

        WaitTime(1.0);
        countdown--;

        if (countdown == 0) {
            isPaused = false;
            break;
        }
    }
}

void Game::run()
{
    InitWindow(screenwidth, screenheight, "METAL SLUG");
    loadTextures();
    healthPack.spawn({ 800.0f,screenheight / 2 + 350 }, true);   // Health pack
    ammoPack.spawn({ 1600.0f, screenheight / 2 + 350 }, false);   // Ammo pack
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (GetKeyPressed() == KEY_ESCAPE)
            break;

        if (!isPaused) {
            update();
        }
    }

    for (int i = 0; i <maxKnifeThrowers; i++) {
        UnloadTexture(knifeThrowers[i].texture);
    }
    UnloadTexture(bigAirship.texture);
    UnloadTexture(background);
    UnloadTexture(background2);
    UnloadTexture(levelThreeBackground);
    UnloadTexture(levelTwoBackground);
    CloseWindow();
}

void Game::drawVictoryScreen()
{
    BeginDrawing();
    ClearBackground(BLACK);

    const char* victoryText = "CONGRATULATIONS!";
    const char* gameEndText = "YOU HAVE COMPLETED THE GAME!";
    const char* finalScoreText = TextFormat("Final Score: %d", score);
    const char* exitText = "Press ESC to Exit";

    int titleFontSize = 60;
    int subtitleFontSize = 40;
    int scoreFontSize = 50;
    int exitFontSize = 30;

    int victoryWidth = MeasureText(victoryText, titleFontSize);
    int gameEndWidth = MeasureText(gameEndText, subtitleFontSize);
    int scoreWidth = MeasureText(finalScoreText, scoreFontSize);
    int exitWidth = MeasureText(exitText, exitFontSize);

    DrawText(victoryText, screenwidth / 2 - victoryWidth / 2,
        screenheight / 2 - 150, titleFontSize, GOLD);

    DrawText(gameEndText, screenwidth / 2 - gameEndWidth / 2,
        screenheight / 2 - 80, subtitleFontSize, GREEN);

    DrawText(finalScoreText, screenwidth / 2 - scoreWidth / 2,
        screenheight / 2 - 20, scoreFontSize, YELLOW);

    DrawText(exitText, screenwidth / 2 - exitWidth / 2,
        screenheight / 2 + 80, exitFontSize, WHITE);

    EndDrawing();
}
