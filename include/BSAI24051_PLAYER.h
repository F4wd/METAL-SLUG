#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "BSAI24051_BULLET.h"
#include "CONSTANTS.h"
const int maxBullets = 100;

class Player
{
public:
    Vector2 PLayerCoordinate;
    Texture2D playerimage[23];
    Texture2D upImage;
    int health;
    bool IsShooting;
    bool IsPLayerMoving;
    bool IsPLayerMovingRIGHT;
    bool IsPLayerMovingLEFT;
    bool IsJumping;
    bool IsOnGround;
    int currentFrame;
    float updateTime;
    float runningTime;
    float jumpVelocity;
    float gravity;
    Bullet bullets[maxBullets];
    bool is_aiming_up;

    Player();
    void loadPlayerImages();
    void initBullets();
    void updateAnimation(float delayTime);
    void updateMovement(int& bgx, int& bg2x);
    void shoot();
    void updateBullets();
    void drawBullets();
    void takeDamage();
    void draw();
    Rectangle getCollider();
};

#endif // PLAYER_H