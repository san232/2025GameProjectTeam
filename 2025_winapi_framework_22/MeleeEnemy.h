#pragma once
#include "BaseEnemy.h"
#include "Collider.h"
#include "Player.h"
#include "Rigidbody.h"
#include <cmath>

class MeleeEnemy : public BaseEnemy
{
public:
    struct DashStat
    {
        float dashDuration = 0.25f;
        float dashImpulse = 700.f;
    };

protected:
    MeleeEnemy(const DashStat& spec)
        : m_dashDuration(spec.dashDuration)
        , m_dashImpulse(spec.dashImpulse)
    {
    }

public:
    void Update() override
    {
        BaseEnemy::Update();

        if (!m_isDashing)
            return;

        m_dashTimer += fDT;
        if (m_dashTimer >= m_dashDuration)
        {
            m_dashTimer = 0.f;
            m_isDashing = false;
            m_hasHitPlayerThisDash = false;

            if (m_rigidbody)
                m_rigidbody->SetVelocity(Vec2(0.f, 0.f));
        }
    }

    void Render(HDC _hdc) override
    {
        BaseEnemy::Render(_hdc);
    }

    void EnterCollision(Collider* _other) override
    {
        if (!m_isDashing || m_hasHitPlayerThisDash || !_other)
            return;

        Object* otherObj = _other->GetOwner();
        Player* player = dynamic_cast<Player*>(otherObj);
        if (!player || player->GetIsDead())
            return;

        player->TakeDamage(GetAttackPower());
        m_hasHitPlayerThisDash = true;
    }

    void StayCollision(Collider* _other) override {}
    void ExitCollision(Collider* _other) override {}

protected:
    void Attack() override
    {
        if (m_isDashing)
            return;

        Player* player = GetTargetPlayer();
        if (!player || player->GetIsDead())
            return;

        Vec2 toPlayer = player->GetPos();
        toPlayer -= GetPos();
        float lenSq = toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y;
        if (lenSq <= 0.f)
            return;

        float len = std::sqrt(lenSq);
        Vec2 dir(toPlayer.x / len, toPlayer.y / len);

        m_isDashing = true;
        m_dashTimer = 0.f;
        m_hasHitPlayerThisDash = false;

        if (m_rigidbody)
        {
            m_rigidbody->SetVelocity(Vec2(0.f, 0.f));
            m_rigidbody->AddImpulse(dir * m_dashImpulse);
        }
    }

    void Dead() override
    {
        BaseEnemy::Dead();
    }

private:
    float m_dashDuration = 0.25f;
    float m_dashTimer = 0.f;
    bool  m_isDashing = false;
    bool  m_hasHitPlayerThisDash = false;
    float m_dashImpulse = 700.f;
};

