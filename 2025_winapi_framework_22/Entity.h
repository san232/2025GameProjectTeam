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
protected:
	virtual void Attack() abstract;
	virtual void Dead() abstract;
	virtual void Move() abstract;
public:
	int GetHp() { return m_hp; }
	void SetHp(int hp) { m_hp = hp; }
	float GetMoveSpeed() { return m_moveSpeed; }
	void SetMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }
	float GetAttackCooltime() { return m_attackCooltime; }
	void SetAttackCooltime(float attackCooltime) { m_attackCooltime = attackCooltime; }
	int GetAttackPower() { return m_attackPower; }
	void SetAttackPower(int attackPower) { m_attackPower = attackPower; }
	void TakeDamage(int damage)
	{
		m_hp -= damage;
		if (m_hp <= 0)
		{
			SetDead();
		}
	}
protected:
	int m_hp;
	float m_moveSpeed;
	float m_attackCooltime;
	int m_attackPower;
};

