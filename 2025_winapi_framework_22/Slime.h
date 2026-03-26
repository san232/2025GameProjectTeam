#pragma once
#include "ProjectileEnemy.h"

class Slime : public ProjectileEnemy
{
public:
    Slime();
    ~Slime() override;

protected:
    void SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir) override;
};
