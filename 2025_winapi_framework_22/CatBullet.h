#pragma once
#include "EnemyBullet.h"

class CatBullet : public EnemyBullet
{
public:
    CatBullet();
    ~CatBullet() override;

protected:
    void OnDead() override;
};
