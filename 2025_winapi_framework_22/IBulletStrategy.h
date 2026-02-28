#pragma once
#include "Vec2.h"

class Entity;
class MirrorBoss;

class IBulletStrategy
{
public:
    virtual ~IBulletStrategy() = default;
    virtual void Fire(MirrorBoss* _boss, Entity* _target, const Vec2& _pos, const Vec2& _dir, int _damage) = 0;
};
