#include "pch.h"
#include "BaseEnemy.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "GDISelector.h"
#include "Defines.h"

BaseEnemy::BaseEnemy()
    : m_position{}
    , m_targetPosition{}
    , m_attackRange(50.f)
{
    AddComponent<Rigidbody>();
    AddComponent<Collider>();
}

BaseEnemy::~BaseEnemy()
{
    SAFE_DELETE(m_stateMachine);
}

void BaseEnemy::Update()
{
    Entity::Update();
    m_position = GetPos();
}

void BaseEnemy::Render(HDC _hdc)
{
    ComponentRender(_hdc);
}

void BaseEnemy::EnterCollision(Collider* _other)
{
}

void BaseEnemy::StayCollision(Collider* _other)
{
}

void BaseEnemy::ExitCollision(Collider* _other)
{
}

void BaseEnemy::MoveToTarget()
{
    Vec2 currentPos = m_position;
    Vec2 targetPos = m_targetPosition;
    Vec2 direction = { targetPos.x - currentPos.x, targetPos.y - currentPos.y };

    if (IsInAttackRange() || (direction.x == 0.f && direction.y == 0.f)) {
        Rigidbody* rigidbody = GetComponent<Rigidbody>();
        if (rigidbody != nullptr) {
            rigidbody->SetVelocity({ 0.f, 0.f });
        }
        return;
    }

    float dist = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (dist > 0.f) {
        direction.x /= dist;
        direction.y /= dist;
    }

    Vec2 velocity = { direction.x * m_moveSpeed, direction.y * m_moveSpeed };
    Rigidbody* rigidbody = GetComponent<Rigidbody>();
    if (rigidbody != nullptr) {
        rigidbody->SetVelocity(velocity);
    }
}

bool BaseEnemy::IsInAttackRange() const
{
    float dx = m_targetPosition.x - m_position.x;
    float dy = m_targetPosition.y - m_position.y;
    float distanceSq = dx * dx + dy * dy;

    return distanceSq <= (m_attackRange * m_attackRange);
}
