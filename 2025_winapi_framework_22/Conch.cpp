#include "pch.h"
#include "Conch.h"

#include "Animator.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "ConchBullet.h"

Conch::Conch()
    : ProjectileEnemy({ 0.3f, 0.48f })
{
    SetHp(10);
    SetMoveSpeed(70.f);
    SetAttackCooltime(1.2f);
    SetAttackRange(220.f);
    SetExp(15);
    SetAttackDelay(0.f);
    SetDefaultLookRight(false);

    SetStatMulti();

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Conch");

    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 15.f }, { 64.f, 64.f }, { 64.f, 0.f }, 11, 0.12f);
    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 80.f }, { 64.f, 64.f }, { 64.f, 0.f }, 4, 0.08f);
    m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 140.f }, { 64.f, 64.f }, { 64.f, 0.f }, 6, 0.06f);
    m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 210.f }, { 64.f, 64.f }, { 64.f, 0.f }, 2, 0.06f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 270.f }, { 64.f, 64.f }, { 64.f, 0.f }, 7, 0.08f);

    m_collider->SetSize({ 30.f, 20.f });
    m_collider->SetOffSetPos({ 5.f, 0.f });
}

void Conch::SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir)
{
    if (!scene || !target || target->GetIsDead())
        return;

    auto* bullet = new ConchBullet;
    bullet->SetPos(origin);
    bullet->SetSize({ 20.f, 20.f });
    bullet->SetDirection(dir);
    bullet->SetTarget(target);

    scene->AddObject(bullet, Layer::ENEMYBULLET);
}
