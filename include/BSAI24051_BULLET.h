#pragma once
#ifndef BSAI24051_BULLET_H
#define BSA24051_BULLET_H

#include "raylib.h"
#include "CONSTANTS.h"
class Bullet
{
public:
    bool active;
    Vector2 position;
    float speed;
    bool enemyB;

    Bullet();
    virtual ~Bullet() = default;
    virtual void update();
    virtual void draw();
};

class HelicopterBullet : public Bullet 
{
public:
    HelicopterBullet();
    void update() override;
    void draw() override;
};

#endif // BULLET_H