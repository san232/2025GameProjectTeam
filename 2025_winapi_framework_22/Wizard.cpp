#include "pch.h"
#include "Wizard.h"

#include "Animator.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "WizardBullet.h"

#include <cmath>

Wizard::Wizard()
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

    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Wizard");

    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 10.f }, { 64.f, 64.f }, { 64.f, 0.f }, 3, 0.1f);
    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 74.f }, { 64.f, 64.f }, { 64.f, 0.f }, 5, 0.1f);
    m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 138.f }, { 64.f, 64.f }, { 64.f, 0.f }, 6, 0.1f);
    m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 202.f }, { 64.f, 64.f }, { 64.f, 0.f }, 2, 0.1f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 266.f }, { 64.f, 64.f }, { 64.f, 0.f }, 7, 0.1f);
}

void Wizard::SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir)
{
    if (!scene || !target || target->GetIsDead())
        return;

    Vec2 normal(-dir.y, dir.x);
    constexpr float spread = 0.4f;

    Vec2 dirs[2] = {
        dir + normal * spread,
        dir - normal * spread
    };

    for (Vec2 shotDir : dirs)
    {
        const float len = std::sqrt(shotDir.x * shotDir.x + shotDir.y * shotDir.y);
        if (len <= 0.f)
            continue;

        shotDir.x /= len;
        shotDir.y /= len;

        auto* bullet = new WizardBullet;
        bullet->SetPos(origin);
        bullet->SetSize({ 20.f, 20.f });
        bullet->SetDirection(shotDir);

        scene->AddObject(bullet, Layer::ENEMYBULLET);
    }
}
