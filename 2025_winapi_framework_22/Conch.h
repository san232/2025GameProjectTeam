#pragma once
#include "ProjectileEnemy.h"

class Conch : public ProjectileEnemy
{
public:
    Conch();
    ~Conch() override;

protected:
    void SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir) override;
};
