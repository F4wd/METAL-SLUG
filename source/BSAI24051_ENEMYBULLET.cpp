#include "BSAI24051_ENEMYBULLET.h"
EnemyBullet::EnemyBullet() : Bullet()
{
    enemyB = true;
    speed = -8.0f;
}
void EnemyBullet:: draw() 
{
    if (active) 
    {
        DrawRectangle(position.x, position.y, 20, 8, YELLOW);
    }
}