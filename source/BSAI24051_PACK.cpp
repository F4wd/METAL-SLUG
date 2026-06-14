#include "BSAI24051_PACK.h"
#include "BSAI24051_PLAYER.h"

extern const int maxBullets;

Pack::Pack() : active(true), isHealth(true), sizeMultiplier(1.0f), timer(0.0f) {}

void Pack::loadTextures(const char* healthImg, const char* ammoImg) {
    image = LoadTexture(healthImg); // pack.png
    altImage = LoadTexture(ammoImg); // tong.png
}

void Pack::spawn(Vector2 pos, bool health) {
    position = pos;
    isHealth = health;
    active = true;
    sizeMultiplier = 1.0f;
    timer = 0.0f;
    updateCollider();
}

void Pack::update(Player& player) {
    if (!active) return;

    // Timer-based grow and descend
    timer += GetFrameTime();
    if (timer >= 10.0f) {
        position.y += 10.0f;
        sizeMultiplier += 0.1f;
        timer = 0.0f;
        updateCollider();
    }

    Rectangle playerRect = player.getCollider();
    if (CheckCollisionRecs(playerRect, collider)) {
        if (isHealth) {
            player.health += 40;
            if (player.health > 100) player.health = 100;
        }
        else {
            for (int i = 0; i < maxBullets; i++) {
                player.bullets[i].active = false;
            }
        }
        active = false;
    }
}

void Pack::updateCollider() {
    float size = 50.0f * sizeMultiplier;
    collider = { position.x, position.y, size, size };
}

void Pack::draw() {
    if (!active) return;

    float size = 50.0f * sizeMultiplier;
    if (isHealth)
        DrawTexturePro(image,
            { 0, 0, (float)image.width, (float)image.height },
            { position.x, position.y, size, size },
            { 0, 0 }, 0, WHITE);
    else
        DrawTexturePro(altImage,
            { 0, 0, (float)altImage.width, (float)altImage.height },
            { position.x, position.y, size, size },
            { 0, 0 }, 0, WHITE);
}