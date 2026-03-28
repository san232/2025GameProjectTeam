#include "pch.h"
#include "MeleeEnemy.h"
#include "Collider.h"
#include "Player.h"
#include "Rigidbody.h"
#include <cmath>

MeleeEnemy::MeleeEnemy(const DashStat& stat)
    : m_dashDuration(stat.dashDuration)
    , m_dashImpulse(stat.dashImpulse)
{
}

void MeleeEnemy::Update()
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

void MeleeEnemy::Render(HDC _hdc)
{
    BaseEnemy::Render(_hdc);
}

void MeleeEnemy::EnterCollision(Collider* _other)
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

void MeleeEnemy::StayCollision(Collider* _other)
{
}

void MeleeEnemy::ExitCollision(Collider* _other)
{
}

void MeleeEnemy::Attack()
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

void MeleeEnemy::Dead()
{
    BaseEnemy::Dead();
}
