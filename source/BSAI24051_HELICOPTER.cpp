#include "BSAI24051_HELICOPTER.h"
#include "BSAI24051_PLAYER.h"
#include <cmath>

extern const int screenwidth;
extern const int screenheight;
extern const int maxBullets;

Helicopter::Helicopter() : Enemy()
{
    speed = 2.0f;
    verticalMovement = 0.0f;
    verticalSpeed = 1.5f;
    rotorAngle = 0.0f;
    movementPattern = 0;
    stateTimer = 0.0f;
    health = 5; // Takes 5 hits to destroy
    shootPattern = 0.0f;
    isDescending = false;
    originalY = 0.0f;
    initHelicopterBullets();
}

void Helicopter::initHelicopterBullets() {
    for (int i = 0; i < 10; i++) {
        helicopterBullets[i].active = false;
    }
}

void Helicopter::shootVertical() {
    for (int i = 0; i < 10; i++) {
        if (!helicopterBullets[i].active) {
            helicopterBullets[i].active = true;
            helicopterBullets[i].position = { position.x + 30, position.y + 50 };
            break;
        }
    }
}

void Helicopter::shootSpread() {
    int bulletsShot = 0;
    for (int i = 0; i < 10 && bulletsShot < 3; i++) {
        if (!helicopterBullets[i].active) {
            helicopterBullets[i].active = true;
            helicopterBullets[i].position = { position.x + 20 + (bulletsShot * 20), position.y + 50 };
            bulletsShot++;
        }
    }
}

void Helicopter::updateMovementPattern() {
    stateTimer += GetFrameTime();

    switch (movementPattern)
    {
    case 0: // Horizontal movement with slight vertical bob
        position.x -= speed;
        position.y += sin(stateTimer * 3.0f) * 0.8f;
        if (stateTimer > 3.0f) {
            movementPattern = 1;
            stateTimer = 0.0f;
            originalY = position.y;
        }
        break;

    case 1: // Descent and hover
        if (!isDescending) {
            position.y += verticalSpeed * 2;
            if (position.y > originalY + 100) {
                isDescending = true;
            }
        }
        else {
            position.x -= speed * 0.5f;
            position.y += sin(stateTimer * 4.0f) * 1.2f;
        }
        if (stateTimer > 4.0f) {
            movementPattern = 2;
            stateTimer = 0.0f;
        }
        break;

    case 2: // Aggressive pursuit
        position.x -= speed * 1.5f;
        position.y += cos(stateTimer * 2.0f) * 2.0f;
        if (stateTimer > 2.5f) {
            movementPattern = 0;
            stateTimer = 0.0f;
            isDescending = false;
        }
        break;
    }
}

void Helicopter::update(Player& player, int& score) {
    if (active) {
        updateMovementPattern();
        rotorAngle += 25.0f; // Fast rotor rotation

        collider.x = position.x;
        collider.y = position.y;

        if (position.x < -150) {
            active = false;
        }

        // Complex shooting pattern
        shootTimer += GetFrameTime();
        shootPattern += GetFrameTime();

        if (shootTimer >= 1.2f) {
            if (sin(shootPattern * 2.0f) > 0) {
                shootSpread();
            }
            else {
                shootVertical();
            }
            shootTimer = 0;
        }

        // Update helicopter bullets
        for (int j = 0; j < 10; j++) {
            if (helicopterBullets[j].active) {
                helicopterBullets[j].update();

                Rectangle bulletRect = { helicopterBullets[j].position.x,
                                       helicopterBullets[j].position.y, 8, 15 };
                Rectangle playerRect = player.getCollider();

                if (CheckCollisionRecs(bulletRect, playerRect)) {
                    player.takeDamage();
                    helicopterBullets[j].active = false;
                }
            }
        }

        // Check collision with player bullets
        for (int j = 0; j < maxBullets; j++) {
            if (player.bullets[j].active) {
                Rectangle bulletCollider = { player.bullets[j].position.x,
                                           player.bullets[j].position.y, 20, 8 };

                if (CheckCollisionRecs(bulletCollider, collider)) {
                    health--;
                    player.bullets[j].active = false;

                    if (health <= 0) {
                        active = false;
                        score += 3; // Higher score for helicopters
                    }
                }
            }
        }
    }
}

void Helicopter::draw(Texture2D& helicopterTexture) {
    if (active) {
        // Draw helicopter body
        DrawTexture(helicopterTexture, position.x, position.y, WHITE);

        // Draw rotating rotor effect
        Vector2 rotorCenter = { position.x + 40, position.y + 10 };
        float rotorLength = 35.0f;

        for (int i = 0; i < 4; i++) {
            float angle = rotorAngle + (i * 90.0f);
            Vector2 rotorEnd = {
                rotorCenter.x + cos(angle * DEG2RAD) * rotorLength,
                rotorCenter.y + sin(angle * DEG2RAD) * rotorLength * 0.3f
            };
            DrawLineEx(rotorCenter, rotorEnd, 3.0f, DARKGRAY);
        }

        // Health indicator
        if (health < 5) {
            DrawRectangle(position.x, position.y - 15, 60, 8, RED);
            DrawRectangle(position.x, position.y - 15, (60 * health) / 5, 8, GREEN);
        }
    }
}

void Helicopter::drawBullets() {
    for (int j = 0; j < 10; j++) {
        helicopterBullets[j].draw();
    }
}

void Helicopter::spawn()
{
    active = true;
    health = 5;

    // Random spawn positions
    int spawnType = GetRandomValue(0, 3);
    switch (spawnType) {
    case 0: // Top right
        position = { (float)(screenwidth + 100), 50 };
        break;
    case 1: // Middle right
        position = { (float)(screenwidth + 100), screenheight / 3 };
        break;
    case 2: // Upper middle
        position = { (float)(screenwidth + 100), screenheight / 4 };
        break;
    case 3: // High altitude
        position = { (float)(screenwidth + 100), 80 };
        break;
    }

    collider = { position.x, position.y, 80, 40 };
    shootTimer = 0;
    movementPattern = GetRandomValue(0, 2);
    stateTimer = 0.0f;
    isDescending = false;
    originalY = position.y;
}