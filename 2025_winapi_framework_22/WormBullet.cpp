#include "pch.h"
#include "WormBullet.h"
#include "EffectManager.h"
#include "WormBulletDeadEffect.h"

WormBullet::WormBullet()
    : EnemyBullet({ L"WormBullet", { 15.f, 15.f }, { 30.f, 30.f }, 350.f, 1, 7.f, 0.4f })
{
}

void WormBullet::OnDead()
{
    GET_SINGLE(EffectManager)->CreateEffect<WormBulletDeadEffect>(GetPos(), { 70.f, 70.f }, 2.f);
}
