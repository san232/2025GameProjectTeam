#pragma once
#include "BaseEnemy.h"

#include <vector>

class MirrorBossIdleState;
class MirrorBossChargeState;
class MirrorBossAttackState;
class IBulletStrategy;

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
	void FireRandomBullet();

	void AddAccumulatedDamage(int damage) { m_accumulatedDamage += damage; }
	bool IsCharging() const { return m_isCharging; }

public:
	State* GetIdleState() const;
	State* GetChargeState() const;
	State* GetAttackState() const;

	float GetIdleDuration() const { return m_idleDuration; }
	float GetChargeDuration() const { return m_chargeDuration; }
	float GetAttackDuration() const { return m_attackStateDuration; }
	float GetAttackSpeed() const { return m_attackSpeed; }
	int GetVolleyCount() const { return m_volleyCount; }

	void UpdateBossFSM();

private:
	void RenderHpUI(HDC _hdc);

private:
	MirrorBossIdleState* m_idleState;
	MirrorBossChargeState* m_chargeState;
	MirrorBossAttackState* m_attackState;

	std::vector<IBulletStrategy*> m_strategies;

	int m_accumulatedDamage;
	bool m_isCharging;
	int m_maxHp;
	
	float m_idleDuration;
	float m_chargeDuration;
	float m_attackStateDuration;

	bool m_isWarningPhase;
	float m_warningDuration;

	bool m_showWarningLine;
	Vec2 m_warningDir;

	// New volley mechanics
	float m_attackSpeed; // Shots per second
	int m_volleyCount;
};
