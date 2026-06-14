
#ifndef BIG_AIRSHIP_H
#define BIG_AIRSHIP_H

#include "raylib.h"
#include "BSAI24051_PLAYER.h"
#include "CONSTANTS.h"

class BigAirship {
public:
    Vector2 position;
    Texture2D texture;
    bool active;
    float angle;
    float radius;
    float shootTimer;
    int health;

    struct SpiralBullet {
        Vector2 position;
        Vector2 velocity;
        bool active;
    } spiralBullets[50];

    BigAirship();
    void loadTexture(const char* filename);
    void spawn();
    void update(Player& player, int& score);
    void fireSpiral();
    void draw();
};

#endif
