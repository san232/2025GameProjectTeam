#pragma once
#include "EnemyBullet.h"

class ConchBullet : public EnemyBullet
{
public:
    ConchBullet();
    ~ConchBullet() override;

protected:
    void OnDead() override;
};
