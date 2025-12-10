#pragma once
#include "BaseEnemy.h"

class MirrorBossIdleState;
class MirrorBossChargeState;
class MirrorBossAttackState;

class MirrorBoss : public BaseEnemy
{
public:
	MirrorBoss();
	~MirrorBoss() override;

	void Update() override;
	void Render(HDC _hdc) override;

	void Move() override;
	void Attack() override;

	void EnterCollision(Collider* _other) override;
	void StayCollision(Collider* _other) override;
	void ExitCollision(Collider* _other) override;

	void OnHit(int damage) override;

	void StartCharge();
	void EndCharge();
	void FireSpecialBullet();

	void AddAccumulatedDamage(int damage) { m_accumulatedDamage += damage; }
	bool IsCharging() const { return m_isCharging; }

public:
	State* GetIdleState() const;
	State* GetChargeState() const;
	State* GetAttackState() const;

	void UpdateBossFSM();

private:
	void RenderHpUI(HDC _hdc);

private:
	MirrorBossIdleState* m_idleState;
	MirrorBossChargeState* m_chargeState;
	MirrorBossAttackState* m_attackState;

	int m_accumulatedDamage;
	bool m_isCharging;
	int m_maxHp;
	
	float m_patternTimer;
	float m_idleDuration;
	float m_chargeDuration;

	float m_attackStateTimer;
	float m_attackStateDuration;
	bool m_hasAttacked;
};
