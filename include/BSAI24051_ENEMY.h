#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "BSAI24051_BULLET.h"
#include "CONSTANTS.h"
#include "BSAI24051_ENEMYBULLET.h"
// Forward declaration to avoid circular dependency
class Player;

class Enemy
{
public:
    bool active;
    Vector2 position;
    float speed;
    Rectangle collider;
    EnemyBullet bullets[5];
    float shootTimer;

    Enemy();
    void initBullets();
    void shoot();
    virtual void update(Player& player, int& score);
    void draw(Texture2D& enemyTexture);
    void drawBullets();
    virtual void spawn();
};

#endif // ENEMY_H