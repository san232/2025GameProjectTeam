#include "pch.h"
#include "Slime.h"

#include "Animator.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SlimeBullet.h"

Slime::Slime()
    : ProjectileEnemy({ 0.2f, 0.48f })
{
    SetHp(10);
    SetMoveSpeed(70.f);
    SetAttackCooltime(2.f);
    SetAttackRange(300.f);
    SetExp(20);
    SetAttackDelay(0.f);
    SetDefaultLookRight(true);

    SetStatMulti();

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Slime");

    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 10.f }, { 64.f, 64.f }, { 64.f, 0.f }, 6, 0.12f);
    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 75.f }, { 64.f, 64.f }, { 64.f, 0.f }, 6, 0.08f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 135.f }, { 64.f, 64.f }, { 64.f, 0.f }, 5, 0.08f);
    m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 200.f }, { 64.f, 64.f }, { 64.f, 0.f }, 7, 0.06f);
    m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 265.f }, { 64.f, 64.f }, { 64.f, 0.f }, 3, 0.06f);

    m_collider->SetOffSetPos({ 0.f, 3.f });
}

void Slime::SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir)
{
    if (!scene)
        return;

    static const Vec2 kDirs[8] = {
        { 0.f, 1.f }, { 1.f, 0.f }, { -1.f, 0.f }, { 0.f, -1.f },
        { 0.70710677f, 0.70710677f }, { 0.70710677f, -0.70710677f },
        { -0.70710677f, 0.70710677f }, { -0.70710677f, -0.70710677f }
    };

    const Vec2 bulletSize = { 20.f, 20.f };

    for (const Vec2& shotDir : kDirs)
    {
        auto* bullet = new SlimeBullet;
        bullet->SetPos(origin);
        bullet->SetSize(bulletSize);
        bullet->SetDirection(shotDir);
        scene->AddObject(bullet, Layer::ENEMYBULLET);
    }
}
