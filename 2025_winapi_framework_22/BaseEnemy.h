#pragma once
#include "Entity.h"
#include "StateMachine.h"

struct Vec2;
class Collider;
class Player;

class EnemyIdleState;
class EnemyMoveState;
class EnemyAttackState;
class EnemyHitState;
class EnemyDeadState;

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

    float GetAttackDelay() const { return m_attackDelay; }
    void  SetAttackDelay(float delay) { m_attackDelay = delay; }

    void MoveToTarget();
    bool IsInAttackRange() const;

    void OnHit(int damage);

    Player* GetTargetPlayer() const { return m_targetPlayer; }

protected:
    void UpdateFSM();
    void UpdateAttackCooldown();
    void UpdateTargetToPlayer();
    Player* FindPlayer() const;
    void SetExp(int exp) { m_exp = exp; }

public:
    void Dead() override;
    void Move() override;

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

    float m_attackDelay;
    float m_attackDelayTimer;
    bool  m_isPreparingAttack;

    Player* m_targetPlayer;

    bool  m_inHitStun;
    float m_hitStunTimer;
    float m_hitStunDuration;

    int m_exp;
};
