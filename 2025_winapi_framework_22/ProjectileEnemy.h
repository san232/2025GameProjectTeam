#pragma once
#include "BaseEnemy.h"
#include "Player.h"
#include "SceneManager.h"

class ProjectileEnemy : public BaseEnemy
{
public:
    struct RangedStat
    {
        float chargeDelay = 0.0f;
        float attackStateDuration = 0.0f;
    };

protected:
    ProjectileEnemy(const RangedStat& spec)
        : m_attackShotDelay(spec.chargeDelay)
    {
        m_attackStateDuration = spec.attackStateDuration;
    }

public:
    void Update() override
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

    void Render(HDC _hdc) override
    {
        BaseEnemy::Render(_hdc);
    }

    void EnterCollision(Collider* _other) override {}
    void StayCollision(Collider* _other) override {}
    void ExitCollision(Collider* _other) override {}

protected:
    void Attack() override
    {
        if (m_isShotCharging)
            return;

        Player* player = GetTargetPlayer();
        if (!player || player->GetIsDead())
            return;

        m_isShotCharging = true;
        m_attackShotTimer = 0.f;
    }

    void Dead() override
    {
        BaseEnemy::Dead();
    }

    bool Aim(Player*& outTarget, Vec2& outOrigin, Vec2& outDir) const
    {
        outTarget = GetTargetPlayer();
        if (!outTarget || outTarget->GetIsDead())
            return false;

        outOrigin = GetPos();
        Vec2 targetPos = outTarget->GetPos();

        outDir = targetPos - outOrigin;
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

    virtual void SpawnProjectiles(Scene* scene, Player* target, const Vec2& origin, const Vec2& dir) = 0;

private:
    float m_attackShotDelay = 0.3f;
    float m_attackShotTimer = 0.f;
    bool m_isShotCharging = false;
};

