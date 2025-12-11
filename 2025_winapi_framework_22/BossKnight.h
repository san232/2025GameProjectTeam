#pragma once
#include "BaseEnemy.h"

class BossChargeState;
class BossDashState;

class BossKnight : public BaseEnemy
{
public:
	BossKnight();
	~BossKnight() override;
    
    	void ClearDashHits() { m_dashHitEntities.clear(); }
    
    
    
    	void Fire8DirectionBullets();
    
    
    
    public:
    
    	void Update() override;
    
    
	void Render(HDC _hdc) override;

	void EnterCollision(Collider* _other) override;
	void StayCollision(Collider* _other) override;
	void ExitCollision(Collider* _other) override;

public:
	BossDashState* GetDashState() const { return m_dashState; }
	State* GetIdleState() const;
	State* GetMoveState() const;
	State* GetAttackState() const;

protected:
	void Attack() override; 
	void Dead() override;

	void OnHit(int damage) override;

private:
		void UpdateBossFSM();
	void RenderBossHpUI(HDC _hdc);
	
	private:
		void ApplyDamage();
	
		BossChargeState* m_chargeState;
		BossDashState* m_dashState;
		Texture* m_hpBarTex;

		int m_maxHp;

		float m_patternCooldown;
		float m_patternTimer;
	
		float m_chargeTimer;
		float m_chargeDuration;
	
		float m_dashTimer;
		float m_dashDuration;
		
		bool m_hasAttacked;
		std::set<Entity*> m_dashHitEntities;
};
		
