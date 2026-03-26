#pragma once
#include "ProjectileEnemy.h"

class Wizard : public ProjectileEnemy
{
public:
    Wizard();
    ~Wizard() override;

protected:
    void SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir) override;
};
