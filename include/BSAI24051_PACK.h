#pragma once
#ifndef PACK_H
#define PACK_H

#include "raylib.h"
#include "CONSTANTS.h"
#include "BSAI24051_PLAYER.h"

// Forward declaration to avoid circular dependency
class Player;

class Pack
{
public:
    Vector2 position;
    Texture2D image;
    Texture2D altImage;
    Rectangle collider;
    bool isHealth; // true = pack.png, false = tong.png
    bool active;
    float sizeMultiplier;
    float timer;

    Pack();
    void loadTextures(const char* healthImg, const char* ammoImg);
    void spawn(Vector2 pos, bool health);
    void update(Player& player);
    void updateCollider();
    void draw();
};

#endif // PACK_H