#include "pch.h"
#include "Necromancer.h"

#include "Animator.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "NecromancerBullet.h"

#include <cmath>

Necromancer::Necromancer()
    : ProjectileEnemy({ 0.3f, 0.48f })
{
    SetHp(10);
    SetMoveSpeed(70.f);
    SetAttackCooltime(0.9f);
    SetAttackRange(300.f);
    SetExp(15);
    SetAttackDelay(0.f);
    SetDefaultLookRight(true);

    SetStatMulti();

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Necromancer");

    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 5.f }, { 64.f, 64.f }, { 64.f, 0.f }, 6, 0.1f);
    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 70.f }, { 64.f, 64.f }, { 64.f, 0.f }, 5, 0.1f);
    m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 130.f }, { 64.f, 64.f }, { 64.f, 0.f }, 3, 0.1f);
    m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 195.f }, { 64.f, 64.f }, { 64.f, 0.f }, 9, 0.06f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 260.f }, { 64.f, 64.f }, { 64.f, 0.f }, 10, 0.05f);

    m_collider->SetSize({ 25.f, 35.f });
}

void Necromancer::SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir)
{
    if (!scene || !target || target->GetIsDead())
        return;

    Vec2 normal(-dir.y, dir.x);
    const float spread = 0.4f;

    Vec2 copyDir = dir;

    Vec2 dirs[3] = {
        copyDir + normal * spread,
        copyDir,
        copyDir - normal * spread
    };

    const Vec2 bulletSize = { 20.f, 20.f };

    for (Vec2 shotDir : dirs)
    {
        const float len = std::sqrt(shotDir.x * shotDir.x + shotDir.y * shotDir.y);
        if (len <= 0.f)
            continue;

        shotDir.x /= len;
        shotDir.y /= len;

        auto* bullet = new NecromancerBullet;
        bullet->SetPos(origin);
        bullet->SetSize(bulletSize);
        bullet->SetDirection(shotDir);
        scene->AddObject(bullet, Layer::ENEMYBULLET);
    }
}
