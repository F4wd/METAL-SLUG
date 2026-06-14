#ifndef HELICOPTER_H
#define HELICOPTER_H

#include "BSAI24051_ENEMY.h"
#include "BSAI24051_BULLET.h"
#include "CONSTANTS.h"
class Helicopter : public Enemy
{
public:
    HelicopterBullet helicopterBullets[10];
    float verticalMovement;
    float verticalSpeed;
    float rotorAngle;
    int movementPattern;
    float stateTimer;
    int health;
    float shootPattern;
    bool isDescending;
    float originalY;

    Helicopter();
    void initHelicopterBullets();
    void shootVertical();
    void shootSpread();
    void updateMovementPattern();
    void update(Player& player, int& score) override;
    void draw(Texture2D& helicopterTexture);
    void drawBullets();
    void spawn() override;
};

#endif // HELICOPTER_H