#pragma once
#include "Entity.h"
#include "StateMachine.h"

struct Vec2;
class Collider;

// BaseEnemy는 Entity를 상속받아 동작.
// Update 흐름은 BaseEnemy에서 고정(템플릿 메서드). 세부 동작은 protected 가상 훅으로 오버라이드.
class BaseEnemy : public Entity
{
public:
    BaseEnemy();
    virtual ~BaseEnemy();

public:
    // 외부에서 호출되는 Update는 BaseEnemy에서 고정된 흐름을 가져야 함.
    void Update() override;
    void Render(HDC _hdc) override;
     
    void EnterCollision(Collider* _other) override;
    void StayCollision(Collider* _other) override;
    void ExitCollision(Collider* _other) override;

public:
    void SetTargetPosition(const Vec2& _targetPos) { m_targetPosition = _targetPos; }

    float GetAttackRange() const { return m_attackRange; }
    const Vec2& GetTargetPosition() const { return m_targetPosition; }

    void MoveToTarget();
    bool IsInAttackRange() const;

protected:
    StateMachine* m_stateMachine = new StateMachine;
    Vec2 m_position;
    Vec2 m_targetPosition;
    float m_attackRange;
};