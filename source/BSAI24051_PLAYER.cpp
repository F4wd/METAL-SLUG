#include "BSAI24051_PLAYER.h"
#include "CONSTANTS.h"
Player::Player()
{
    PLayerCoordinate.x = screenwidth / 2 - 550;
    PLayerCoordinate.y = screenheight / 2 + 350;
    updateTime = 0.1f;
    runningTime = 0.0f;
    currentFrame = 0;
    jumpVelocity = 0.0f;
    gravity = 1.0f;
    health = 100;
    IsShooting = false;
    IsPLayerMoving = false;
    IsPLayerMovingRIGHT = false;
    IsPLayerMovingLEFT = false;
    IsJumping = false;
    IsOnGround = true;
    is_aiming_up = false;
    initBullets();
}

void Player::loadPlayerImages() {
    playerimage[0] = LoadTexture("0.png");
    playerimage[1] = LoadTexture("1.png");
    playerimage[2] = LoadTexture("2.png");
    playerimage[3] = LoadTexture("3.png");
    playerimage[4] = LoadTexture("4.png");
    playerimage[5] = LoadTexture("5.png");
    playerimage[6] = LoadTexture("6.png");
    playerimage[7] = LoadTexture("7.png");
    playerimage[8] = LoadTexture("8.png");
    playerimage[9] = LoadTexture("9.png");
    playerimage[10] = LoadTexture("10.png");
    playerimage[11] = LoadTexture("11.png");
    playerimage[12] = LoadTexture("12.png");
    playerimage[13] = LoadTexture("13.png");
    playerimage[14] = LoadTexture("14.png");
    playerimage[15] = LoadTexture("15.png");
    playerimage[16] = LoadTexture("16.png");
    playerimage[17] = LoadTexture("17.png");
    playerimage[18] = LoadTexture("18.png");
    playerimage[19] = LoadTexture("19.png");
    playerimage[20] = LoadTexture("20.png");
    playerimage[21] = LoadTexture("21.png");
    playerimage[22] = LoadTexture("22.png");

    upImage = LoadTexture("up.png");
    upImage.height = screenheight / 4 - 150;
    upImage.width = screenwidth / 4 - 150;
    for (int i = 0; i < 23; i++) {
        playerimage[i].height = screenheight / 4 - 150;
        playerimage[i].width = screenwidth / 4 - 150;
    }
}

void Player::initBullets()
{
    for (int i = 0; i < maxBullets; i++) {
        bullets[i].active = false;
        bullets[i].speed = 10.0f;
    }
}

void Player::updateAnimation(float delayTime) {
    runningTime += delayTime;
    if (runningTime >= updateTime) {
        runningTime = 0.0f;
        currentFrame++;
        if (currentFrame > 22)
            currentFrame = 0;
    }
}

void Player::updateMovement(int& bgx, int& bg2x)
{
    // Aiming Up
    is_aiming_up = IsKeyDown(KEY_W);  // Press W to aim up

    // Horizontal Movement
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        PLayerCoordinate.x += 3.0f;
        IsPLayerMoving = true;
        IsPLayerMovingLEFT = false;
        IsPLayerMovingRIGHT = true;
    }
    else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && bgx < 0)
    {
        if (PLayerCoordinate.x > 0)
        {
            PLayerCoordinate.x -= 3.0f;
            IsPLayerMoving = true;
            IsPLayerMovingLEFT = true;
            IsPLayerMovingRIGHT = false;
        }
    }
    else
    {
        IsPLayerMoving = false;
        IsPLayerMovingLEFT = false;
        IsPLayerMovingRIGHT = false;
    }

    // Jumping
    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) && IsOnGround) {
        IsJumping = true;
        jumpVelocity = -15.0f;
        IsOnGround = false;
    }

    // Apply Gravity
    if (!IsOnGround)
    {
        PLayerCoordinate.y += jumpVelocity;
        jumpVelocity += gravity;
        if (PLayerCoordinate.y >= screenheight / 2 + 350)
        {
            PLayerCoordinate.y = screenheight / 2 + 350;
            IsOnGround = true;
            IsJumping = false;
        }
    }
}

void Player::shoot() {
    for (int i = 0; i < maxBullets; i++) {
        if (!bullets[i].active) {
            bullets[i].active = true;

            if (is_aiming_up)
            {
                bullets[i].position = { PLayerCoordinate.x + 30, PLayerCoordinate.y };
                bullets[i].speed = -10.0f; // Shoot vertically upwards
            }
            else {
                bullets[i].position = { PLayerCoordinate.x + 50, PLayerCoordinate.y + 20 };
                bullets[i].speed = 10.0f; // Shoot horizontally
            }

            break;
        }
    }
}

void Player::updateBullets() {
    for (int i = 0; i < maxBullets; i++) {
        bullets[i].update();
    }
}

void Player::drawBullets() {
    for (int i = 0; i < maxBullets; i++) {
        bullets[i].draw();
    }
}

void Player::takeDamage() {
    health -= 1;
}

void Player::draw()
{
    if (is_aiming_up)
    {
        DrawTexture(upImage, PLayerCoordinate.x, PLayerCoordinate.y, WHITE);
    }
    else if (IsPLayerMoving) {
        DrawTexture(playerimage[currentFrame], PLayerCoordinate.x, PLayerCoordinate.y, WHITE);
    }
    else {
        DrawTexture(playerimage[0], PLayerCoordinate.x, PLayerCoordinate.y, WHITE);
    }
}

Rectangle Player::getCollider() {
    return { PLayerCoordinate.x + 20, PLayerCoordinate.y + 20,
            (float)playerimage[0].width - 40, (float)playerimage[0].height - 40 };
}