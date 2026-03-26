#include "pch.h"
#include "ConchBullet.h"
#include "EffectManager.h"
#include "ConchBulletDeadEffect.h"

ConchBullet::ConchBullet()
    : EnemyBullet({ L"ConchBullet", { 15.f, 15.f }, { 20.f, 20.f }, 250.f, 1, 2.f, 3.5f })
{
}

void ConchBullet::OnDead()
{
    GET_SINGLE(EffectManager)->CreateEffect<ConchBulletDeadEffect>(GetPos(), { 70.f, 70.f }, 2.f);
}
