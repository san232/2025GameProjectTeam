#pragma once
#include "ProjectileEnemy.h"

class Worm : public ProjectileEnemy
{
public:
    Worm();
    ~Worm() override;

protected:
    void SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir) override;
};
