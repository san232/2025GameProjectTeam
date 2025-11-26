#include "pch.h"
#include "BaseEnemy.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "GDISelector.h"
#include "Defines.h"

BaseEnemy::BaseEnemy()
    : m_position{}
    , m_targetPosition{}
    , m_health(100)
    , m_moveSpeed(100.f)
    , m_attackSpeed(1.f)
    , m_timeSinceLastAttack(0.f)
    , m_attackPower(10)
    , m_attackRange(50.f)
    , m_deltaTime(0.f)
{
    AddComponent<Rigidbody>();
    AddComponent<Collider>();
}

BaseEnemy::~BaseEnemy()
{
}

void BaseEnemy::Update()
{
    float deltaTime = fDT;
    m_deltaTime = deltaTime;
    m_timeSinceLastAttack += deltaTime;

    m_position = GetPos();

    MoveToTarget(deltaTime);
    TryAttack(deltaTime);

    if (m_health <= 0) {
        Death();
    }
}

void BaseEnemy::Render(HDC _hdc)
{
    BrushType brush = BrushType::RED;
    PenType pen = PenType::RED;

    GDISelector brushSelector(_hdc, brush);
    GDISelector penSelector(_hdc, pen);

    Vec2 pos = GetPos();
    Vec2 size = GetSize();
    RECT_RENDER(_hdc, pos.x, pos.y, size.x, size.y);

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

void BaseEnemy::TakeDamage(int damage)
{
    //이거 인터페이스로 분리 해야할듯.
    m_health -= damage;
    if (m_health <= 0) {
        Death();
    }
}

void BaseEnemy::MoveToTarget(float deltaTime)
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

void BaseEnemy::TryAttack(float deltaTime)
{
    if (IsInAttackRange()) {
        float attackInterval = (m_attackSpeed > 0.f) ? (1.0f / m_attackSpeed) : FLT_MAX;
        if (m_timeSinceLastAttack >= attackInterval) {
            m_timeSinceLastAttack = 0.f;
        }
    }
}

bool BaseEnemy::IsInAttackRange() const
{
    float dx = m_targetPosition.x - m_position.x;
    float dy = m_targetPosition.y - m_position.y;
    float distanceSq = dx * dx + dy * dy;

    return distanceSq <= (m_attackRange * m_attackRange);
}

void BaseEnemy::Death()
{
}
