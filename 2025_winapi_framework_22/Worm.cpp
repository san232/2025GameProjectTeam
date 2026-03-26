#include "pch.h"
#include "Worm.h"

#include "Animator.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "WormBullet.h"

Worm::Worm()
    : ProjectileEnemy({ 0.3f, 0.48f })
{
    SetHp(15);
    SetMoveSpeed(70.f);
    SetAttackCooltime(1.2f);
    SetAttackRange(300.f);
    SetExp(15);
    SetAttackDelay(0.f);
    SetDefaultLookRight(true);

    SetStatMulti();

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Worm");

    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 64.f, 0.f }, 4, 0.12f);
    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 60.f }, { 64.f, 64.f }, { 64.f, 0.f }, 4, 0.08f);
    m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 120.f }, { 64.f, 64.f }, { 64.f, 0.f }, 5, 0.06f);
    m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 180.f }, { 64.f, 64.f }, { 64.f, 0.f }, 7, 0.06f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 240.f }, { 64.f, 64.f }, { 64.f, 0.f }, 6, 0.08f);

    m_collider->SetSize({ 45.f, 35.f });
}

void Worm::SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir)
{
    if (!scene || !target || target->GetIsDead())
        return;

    auto* bullet = new WormBullet;
    bullet->SetPos(origin);
    bullet->SetSize({ 30.f, 30.f });
    bullet->SetDirection(dir);
    bullet->SetTarget(target);

    scene->AddObject(bullet, Layer::ENEMYBULLET);
}
