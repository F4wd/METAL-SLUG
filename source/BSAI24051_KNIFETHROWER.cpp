#include "BSAI24051_KNIFETHROWER.h"

KnifeThrower::KnifeThrower() : Enemy() {
    speed = 3.0f;           // Horizontal speed (left)
    verticalSpeed = 1.5f;   // Vertical descent speed (downward)
}

void KnifeThrower::loadTexture(const char* filename) {
    texture = LoadTexture(filename);
}

void KnifeThrower::update(Player& player, int& score) {
    if (active) {
        // Move diagonally down-left
        position.x -= speed;
        position.y += verticalSpeed;

        collider.x = position.x;
        collider.y = position.y - 40;

        // Despawn if off screen
        if (position.x < -100 || position.y > screenheight) 
        {
            active = false;
        }

        // If touches player → game over
        if (CheckCollisionRecs(collider, player.getCollider())) {
            player.health = 0;
        }

        // If hit by bullet → die and increase score
        for (int j = 0; j < maxBullets; j++) {
            if (player.bullets[j].active) {
                Rectangle bulletCollider = { player.bullets[j].position.x,
                                             player.bullets[j].position.y,
                                             20, 8 };

                if (CheckCollisionRecs(bulletCollider, collider)) {
                    active = false;
                    player.bullets[j].active = false;
                    score++;
                }
            }
        }
    }
}

void KnifeThrower::draw() {
    if (active) {
        // Scale down to 70% size and draw slightly upward
        float scale = 0.7f;
        float width = texture.width * scale;
        float height = texture.height * scale;

        DrawTexturePro(
            texture,
            { 0, 0, (float)texture.width, (float)texture.height },     // source
            { position.x, position.y - 40, width, height },            // destination (shifted up)
            { 0, 0 }, 0.0f, WHITE
        );
    }
}

void KnifeThrower::spawn() {
    active = true;
    position = { (float)(screenwidth + 100), (float)GetRandomValue(50, 300) }; // Spawn in air
    collider = { position.x, position.y - 40, 80, 60 };
    shootTimer = 0;
}