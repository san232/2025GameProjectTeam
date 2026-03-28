#include "pch.h"
#include "ProjectileEnemy.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Player.h"
#include <cmath>

ProjectileEnemy::ProjectileEnemy(const RangedStat& stat)
    : m_attackShotDelay(stat.chargeDelay)
{
    m_attackStateDuration = stat.attackStateDuration;
}

void ProjectileEnemy::Update()
{
    BaseEnemy::Update();

    if (!m_isShotCharging)
        return;

    m_attackShotTimer += fDT;
    if (m_attackShotTimer < m_attackShotDelay)
        return;

    m_attackShotTimer = 0.f;
    m_isShotCharging = false;

    auto scene = GET_SINGLE(SceneManager)->GetCurScene();
    if (!scene)
        return;

    Player* target = nullptr;
    Vec2 origin{};
    Vec2 dir{};
    if (!Aim(target, origin, dir))
        return;

    SpawnProjectiles(scene.get(), target, origin, dir);
}

void ProjectileEnemy::Render(HDC _hdc)
{
    BaseEnemy::Render(_hdc);
}

void ProjectileEnemy::EnterCollision(Collider* _other)
{
}

void ProjectileEnemy::StayCollision(Collider* _other)
{
}

void ProjectileEnemy::ExitCollision(Collider* _other)
{
}

void ProjectileEnemy::Attack()
{
    if (m_isShotCharging)
        return;

    Player* player = GetTargetPlayer();
    if (!player || player->GetIsDead())
        return;

    m_isShotCharging = true;
    m_attackShotTimer = 0.f;
}

void ProjectileEnemy::Dead()
{
    BaseEnemy::Dead();
}

bool ProjectileEnemy::Aim(Player*& outTarget, Vec2& outOrigin, Vec2& outDir) const
{
    outTarget = GetTargetPlayer();
    if (!outTarget || outTarget->GetIsDead())
        return false;

    outOrigin = GetPos();

    Vec2 targetPos = outTarget->GetPos();
    outDir = targetPos;
    outDir -= outOrigin;

    const float len = std::sqrt(outDir.x * outDir.x + outDir.y * outDir.y);
    if (len > 0.f)
    {
        outDir.x /= len;
        outDir.y /= len;
    }
    else
    {
        outDir = Vec2{ 1.f, 0.f };
    }

    return true;
}
