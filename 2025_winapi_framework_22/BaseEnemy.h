#pragma once
#include "Object.h"
struct Vec2;
class BaseEnemy :
    public Object
{
public:
    BaseEnemy();
    virtual ~BaseEnemy();

public:
    void Update() override;
    void Render(HDC _hdc) override;

    void EnterCollision(Collider* _other)override;
    void StayCollision(Collider* _other) override;
    void ExitCollision(Collider* _other) override;

	void TakeDamage(int damage);
public:
	void SetHealth(int _health) { m_health = _health; }
	void SetMoveSpeed(float _speed) { m_moveSpeed = _speed; }
	void SetAttackSpeed(float _speed) { m_attackSpeed = _speed; }
	void SetAttackPower(int _power) { m_attackPower = _power; }
	int GetHealth() const { return m_health; }
	float GetMoveSpeed() const { return m_moveSpeed; }
	float GetAttackSpeed() const { return m_attackSpeed; }
	int GetAttackPower() const { return m_attackPower; }
	float GetAttackRange() const { return m_attackRange; }
	const Vec2& GetTargetPosition() const { return m_targetPosition; }
	void SetTargetPosition(const Vec2& _targetPos) { m_targetPosition = _targetPos; }
protected:
	virtual void Death();
	void MoveToTarget(float deltaTime);
	void TryAttack(float deltaTime);
	bool IsInAttackRange() const;
private:
	Vec2 m_position;
	Vec2 m_targetPosition;
    int m_health;
    float m_moveSpeed;
    float m_attackSpeed;
    float m_timeSinceLastAttack;
    int m_attackPower;
	float m_attackRange;
};

