#include "BSAI24051_BIGAIRSHIP.h"
#include <cmath>
BigAirship::BigAirship() {
    active = false;
    angle = 0.0f;
    radius = 0.0f;
    shootTimer = 0.0f;
    health = 20; // More hits to defeat

    for (int i = 0; i < 50; i++) {
        spiralBullets[i].active = false;
    }
}

void BigAirship::loadTexture(const char* filename) {
    texture = LoadTexture(filename);
}

void BigAirship::spawn() {
    active = true;
    angle = 0.0f;
    radius = 0.0f;
    health = 20;
    position = { (float)(screenwidth / 2), 100 };
}

void BigAirship::update(Player& player, int& score) {
    if (!active) return;

    // Spiral movement
    angle += 0.02f;
    radius += 0.1f;
    position.x = screenwidth / 2 + cos(angle) * radius;
    position.y = 100 + sin(angle * 1.5f) * radius * 0.5f;

    // Fire bullets outward in spiral
    shootTimer += GetFrameTime();
    if (shootTimer >= 0.6f) {
        fireSpiral();
        shootTimer = 0.0f;
    }

    // Update spiral bullets
    for (int i = 0; i < 50; i++) {
        if (spiralBullets[i].active) {
            spiralBullets[i].position.x += spiralBullets[i].velocity.x;
            spiralBullets[i].position.y += spiralBullets[i].velocity.y;

            if (spiralBullets[i].position.x < 0 || spiralBullets[i].position.x >screenwidth ||
                spiralBullets[i].position.y < 0 || spiralBullets[i].position.y > screenheight) {
                spiralBullets[i].active = false;
            }

            Rectangle bulletRect = { spiralBullets[i].position.x, spiralBullets[i].position.y, 6, 6 };
            if (CheckCollisionRecs(bulletRect, player.getCollider())) {
                player.takeDamage();
                spiralBullets[i].active = false;
            }
        }
    }

    // Check for player bullets hitting airship
    for (int j = 0; j < maxBullets; j++) {
        if (player.bullets[j].active) {
            Rectangle bulletCollider = {
                player.bullets[j].position.x, player.bullets[j].position.y, 20, 8
            };
            Rectangle airshipCollider = {
                position.x, position.y, (float)texture.width, (float)texture.height
            };

            if (CheckCollisionRecs(bulletCollider, airshipCollider)) {
                health--;
                player.bullets[j].active = false;

                if (health <= 0) {
                    active = false;
                    score += 5; // Airship gives more score
                }
            }
        }
    }
}

void BigAirship::fireSpiral() {
    const int numShots = 20;
    float cx = position.x + texture.width / 2;
    float cy = position.y + texture.height / 2;

    for (int i = 0; i < 50; i++) {
        if (!spiralBullets[i].active) {
            float angleDeg = (i % numShots) * (360.0f / numShots);
            float rad = angleDeg * DEG2RAD;

            spiralBullets[i].position = { cx, cy };
            spiralBullets[i].velocity = { cos(rad) * 4.0f, sin(rad) * 4.0f };
            spiralBullets[i].active = true;
        }
    }
}

void BigAirship::draw() {
    if (!active) return;

    DrawTexture(texture, position.x, position.y, WHITE);

    // Draw health bar above airship
    float barWidth = 100;
    float healthPercent = (float)health / 20.0f;
    DrawRectangle(position.x, position.y - 20, barWidth, 8, RED);
    DrawRectangle(position.x, position.y - 20, barWidth * healthPercent, 8, GREEN);

    for (int i = 0; i < 50; i++) {
        if (spiralBullets[i].active) {
            DrawCircleV(spiralBullets[i].position, 4, ORANGE);
        }
    }
}