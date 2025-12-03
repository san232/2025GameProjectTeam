#pragma once
#include "Entity.h"
#include "Vec2.h"

class StateMachine;
class EnemyIdleState;
class EnemyMoveState;
class EnemyAttackState;
class EnemyHitState;
class EnemyDeadState;

class Collider;
class Player;

class BaseEnemy : public Entity
{
public:
    BaseEnemy();
    virtual ~BaseEnemy();

public:
    void Update() override;
    void Render(HDC _hdc) override;

    void EnterCollision(Collider* _other) override;
    void StayCollision(Collider* _other) override;
    void ExitCollision(Collider* _other) override;

public:
    void SetTargetPosition(const Vec2& _targetPos) { m_targetPosition = _targetPos; }

    float GetAttackRange() const { return m_attackRange; }
    const Vec2& GetTargetPosition() const { return m_targetPosition; }
    void SetAttackRange(float range) { m_attackRange = range; }

    void MoveToTarget();
    bool IsInAttackRange() const;

    void OnHit(int damage);

protected:
    void UpdateFSM();
    void UpdateAttackCooldown();
    void UpdateTargetToPlayer();
    Player* FindPlayer() const;

public:
    void Dead() override;
    void Move() override { MoveToTarget(); }

protected:
    StateMachine* m_stateMachine;

    EnemyIdleState* m_idleState;
    EnemyMoveState* m_moveState;
    EnemyAttackState* m_attackState;
    EnemyHitState* m_hitState;
    EnemyDeadState* m_deadState;

    Vec2  m_position;
    Vec2  m_targetPosition;
    float m_attackRange;

    float m_attackTimer;
    bool  m_canAttack;

    Player* m_targetPlayer;

    bool  m_inHitStun;
    float m_hitStunTimer;
    float m_hitStunDuration;
};
