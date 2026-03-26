#include "pch.h"
#include "Snail.h"

#include "Animator.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SnailBullet.h"

Snail::Snail()
    : ProjectileEnemy({ 0.2f, 0.25f })
{
    SetHp(5);
    SetMoveSpeed(70.f);
    SetAttackCooltime(0.5f);
    SetAttackRange(300.f);
    SetExp(15);
    SetAttackDelay(0.f);
    SetDefaultLookRight(false);

    SetStatMulti();

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Snail");

    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 7.f }, { 64.f, 30.f }, { 64.f, 0.f }, 2, 0.12f);
    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 35.f }, { 64.f, 30.f }, { 64.f, 0.f }, 4, 0.08f);
    m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 62.f }, { 64.f, 30.f }, { 64.f, 0.f }, 7, 0.06f);
    m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 90.f }, { 64.f, 30.f }, { 64.f, 0.f }, 2, 0.06f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 120.f }, { 64.f, 30.f }, { 64.f, 0.f }, 10, 0.08f);

    m_collider->SetSize({ 30.f, 25.f });
    m_collider->SetOffSetPos({ 0.f, 10.f });
}

void Snail::SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir)
{
    if (!scene || !target || target->GetIsDead())
        return;

    auto* bullet = new SnailBullet;
    bullet->SetPos(origin);
    bullet->SetSize({ 20.f, 20.f });
    bullet->SetDirection(dir);

    scene->AddObject(bullet, Layer::ENEMYBULLET);
}
