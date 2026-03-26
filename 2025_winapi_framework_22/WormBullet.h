#pragma once
#include "EnemyBullet.h"

class WormBullet : public EnemyBullet
{
public:
    WormBullet();
    ~WormBullet() override;

protected:
    void OnDead() override;
};
