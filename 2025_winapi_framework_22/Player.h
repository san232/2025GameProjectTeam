#pragma once
#include "Entity.h"
#include "Enums.h"
#include "StateMachine.h"

class Texture;
class Collider;
class Rigidbody;
class Animator;
class PlayerIdleState;
class PlayerMoveState;
class PlayerRollingState;
class PlayerDeadState;

class Player : public Entity
{
public:
    Player();
    ~Player();

public:
    void Update() override;
    void Render(HDC _hdc) override;
    void EnterCollision(Collider* _other) override;
    void StayCollision(Collider* _other) override;
    void ExitCollision(Collider* _other) override;

private:
    void RenderExpUI(HDC _hdc);
    void RenderHpUI(HDC _hdc);
    void UpdateInput();
    void CooldownRollingTime();
    void CooldownAttackTime();
    void BlockPlayer();

public:
    void TakeDamage(int _damage) override;
    void SetInvincibility(bool isInvincibility) { m_isInvincibility = isInvincibility; }
    void StopMoving();
    void Rolling();
    void ChangeAnimation(wstring animationName, bool isLoop = true);
    int LevelUp(int level);
    void TakeExp(int exp);
    void Destroy();

private:
    PlayerIdleState* m_idleState;
    PlayerMoveState* m_moveState;
    PlayerRollingState* m_rollingState;
    PlayerDeadState* m_deadState;

private:
    StateMachine* m_stateMachine = new StateMachine;
    Texture* m_pTex;
    Rigidbody* m_rigidCompo;
    Vec2 m_moveDirection;
    Animator* m_animator;
    float m_dashPower;
    float m_curTime;
    int m_maxHp;

    bool m_canRolling;
    float m_rollingStateRemainTime;
    float m_rollingCooltime;

    bool m_canAttack;
    float m_attackElapsedTime;

    int m_level;
    int m_curExp;
    int m_needExp;

    bool m_isInvincibility = false;

    bool m_isDeadState = false;

public:
    void Attack() override;
    void Dead() override;
    void Move() override;
};
