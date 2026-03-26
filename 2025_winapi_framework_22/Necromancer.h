#pragma once
#include "ProjectileEnemy.h"

class Necromancer : public ProjectileEnemy
{
public:
    Necromancer();
    ~Necromancer() override;

protected:
    void SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir) override;
};