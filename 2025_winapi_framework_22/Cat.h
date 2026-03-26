#pragma once
#include "ProjectileEnemy.h"

class Cat : public ProjectileEnemy
{
public:
    Cat();
    ~Cat() override;

protected:
    void SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir) override;
};
