#pragma once
#include "BaseEnemy.h"

class MirrorBossIdleState;
class MirrorBossChargeState;
class MirrorBossAttackState;

enum class BossAttackType
{
	FAST,
	HOMING,
	BOOMERANG,
	FRAGMENTING,
	MAX_TYPES
};

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

	bool m_isWarningPhase;
	float m_warningTimer;
	float m_warningDuration;

	bool m_showWarningLine;
	Vec2 m_warningDir;

	// New volley mechanics
	float m_attackSpeed; // Shots per second
	int m_volleyCount;
	int m_bulletsFiredInVolley;
	float m_timeSinceLastShot;
};
