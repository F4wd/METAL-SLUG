#include "BSAI24051_BULLET.h"


Bullet::Bullet() : active(false), speed(10.0f), enemyB(false) {}

void Bullet::update()
{
    if (!enemyB)
    {
        if (active)
        {
            if (speed > 0)
            {
                position.x += speed;  // rightward (horizontal)
            }
            else {
                position.y += speed;  // upward (vertical since speed is negative)
            }
        }
    }
    else
    {
        if (active)
        {
            position.x += speed;
        }
    }
}

void Bullet::draw() {
    if (active) {
        DrawRectangle(position.x + 50, position.y + 25, 20, 8, RED);
    }
}

HelicopterBullet::HelicopterBullet() : Bullet() {
    speed = 0.0f; // Vertical movement only
}

void HelicopterBullet::update() {
    if (active) {
        position.y += 12.0f; // Fast downward movement
        if (position.y > screenheight) {
            active = false;
        }
    }
}

void HelicopterBullet::draw() {
    if (active) {
        DrawRectangle(position.x, position.y, 8, 15, ORANGE);
        DrawCircle(position.x + 4, position.y + 7, 3, RED); // Explosive tip
    }
}