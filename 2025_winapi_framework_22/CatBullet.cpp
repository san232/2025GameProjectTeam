#include "pch.h"
#include "CatBullet.h"
#include "EffectManager.h"
#include "CatBulletDeadEffect.h"

CatBullet::CatBullet()
    : EnemyBullet({ L"CatBullet", { 10.f, 10.f }, { 20.f, 20.f }, 200.f, 1, 5.f, 1.5f })
{
}

void CatBullet::OnDead()
{
    GET_SINGLE(EffectManager)->CreateEffect<CatBulletDeadEffect>(GetPos(), { 70.f, 70.f }, 2.f);
}
