#ifndef KNIFE_THROWER_H
#define KNIFE_THROWER_H

#include "raylib.h"
#include "BSAI24051_ENEMY.h"
#include "BSAI24051_PLAYER.h"
#include "CONSTANTS.h"

class KnifeThrower : public Enemy {
public:
    Texture2D texture;
    float verticalSpeed;

    KnifeThrower();
    void loadTexture(const char* filename);
    void update(Player& player, int& score) override;
    void draw();
    void spawn() override;
};

#endif