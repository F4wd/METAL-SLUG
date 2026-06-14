#include "BSAI24051_ENEMY.h"
#include "BSAI24051_PLAYER.h"
#include "CONSTANTS.h"



Enemy::Enemy()
{
    active = false;
    speed = 3.0f;
    shootTimer = 0;
    initBullets();
}

void Enemy::initBullets() {
    for (int j = 0; j < 5; j++) {
        bullets[j].active = false;
        bullets[j].speed = -8.0f;
    }
}

void Enemy::shoot() {
    for (int i = 0; i < 5; i++) {
        if (!bullets[i].active) {
            bullets[i].active = true;
            bullets[i].position = { position.x, position.y + 30 };
            break;
        }
    }
}

void Enemy::update(Player& player, int& score) {
    if (active) {
        position.x -= speed;
        collider.x = position.x;

        if (position.x < -100) {
            active = false;
        }

        shootTimer += GetFrameTime();
        if (shootTimer >= 2.0f) {
            shoot();
            shootTimer = 0;
        }

        for (int j = 0; j < 5; j++) {
            if (bullets[j].active) {
                bullets[j].update();

                Rectangle bulletRect = { bullets[j].position.x, bullets[j].position.y, 20, 8 };
                Rectangle playerRect = player.getCollider();

                if (CheckCollisionRecs(bulletRect, playerRect)) {
                    player.takeDamage();
                    bullets[j].active = false;
                }
            }
        }

        for (int j = 0; j < maxBullets; j++) {
            if (player.bullets[j].active) {
                Rectangle bulletCollider = { player.bullets[j].position.x,
                                           player.bullets[j].position.y, 20, 8 };

                if (CheckCollisionRecs(bulletCollider, collider)) {
                    active = false;
                    player.bullets[j].active = false;
                    score++;
                }
            }
        }
    }
}

void Enemy::draw(Texture2D& enemyTexture) {
    if (active) {
        DrawTexture(enemyTexture, position.x, position.y, WHITE);
    }
}

void Enemy::drawBullets() {
    for (int j = 0; j < 5; j++) {
        bullets[j].draw();
    }
}

void Enemy::spawn()
{
    active = true;
    position = { (float)(screenwidth + 100), screenheight / 2 + 350 };
    collider = { position.x, position.y, 100, 60 };
    shootTimer = 0;
}