#pragma once
#include "ProjectileEnemy.h"

class Snail : public ProjectileEnemy
{
public:
    Snail();
    ~Snail() override;

protected:
    void SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir) override;
};
