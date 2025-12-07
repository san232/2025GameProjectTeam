#pragma once
#include "Object.h"

class Entity : public Object
{
public:
	Entity();
	~Entity();
public:
	// Object을(를) 통해 상속됨
	void Update() override;
	void Render(HDC _hdc) override;
public:
	virtual void Attack() abstract;
	virtual void Dead() abstract;
	virtual void Move() abstract;
public:
	int GetHp() { return m_hp; }
	void SetHp(int _hp) { m_hp = _hp; }
	float GetMoveSpeed() { return m_moveSpeed; }
	void SetMoveSpeed(float _moveSpeed) { m_moveSpeed = _moveSpeed; }
	float GetAttackCooltime() { return m_attackCooltime; }
	void SetAttackCooltime(float _attackCooltime) { m_attackCooltime = _attackCooltime; }
	int GetAttackPower() { return m_attackPower; }
	void SetAttackPower(int _attackPower) { m_attackPower = _attackPower; }
	virtual void TakeDamage(int _damage)
	{
		m_hp -= _damage;
		if (m_hp <= 0)
		{
			Dead();
		}
	}
protected:
	int m_hp;
	float m_moveSpeed;
	float m_attackCooltime;
	int m_attackPower;
};

