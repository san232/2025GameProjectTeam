#include "pch.h"
#include "MirrorBoss.h"
#include "State.h"
#include "StateMachine.h"
#include "Player.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Animator.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "GDISelector.h"
#include "EffectManager.h"
#include "EnemyHitEffect.h"
#include "MirrorBossIdleState.h"
#include "MirrorBossChargeState.h"
#include "MirrorBossAttackState.h"
#include "FastBullet.h"
#include "HomingBullet.h"
#include "BoomerangBullet.h"
#include "FragmentingBullet.h"


MirrorBoss::MirrorBoss()
	: m_accumulatedDamage(0)
	, m_isCharging(false)
	, m_maxHp(500)
	, m_patternTimer(0.f)
	, m_idleDuration(2.0f)
	, m_chargeDuration(3.0f)
	, m_attackStateTimer(0.f)
	, m_attackStateDuration(2.0f)
	, m_isWarningPhase(false)
	, m_warningTimer(0.f)
	, m_warningDuration(1.0f)
	, m_showWarningLine(false)
	, m_warningDir{}
	, m_attackSpeed(4.f)
	, m_volleyCount(5)
	, m_bulletsFiredInVolley(0)
	, m_timeSinceLastShot(0.f)
{
	SetHp(m_maxHp);
	SetMoveSpeed(0.f);
	SetAttackPower(10);
	SetExp(1000);
	
	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"MirrorBoss"); 

	m_animator->SetScaleRatio(3.0f);
	m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f,  0.f }, { 128.f, 128.f }, { 128.f, 0.f }, 7, 0.15f);
	m_animator->CreateAnimation(L"Charge", m_pTex, { 0.f,  128.f }, { 128.f, 128.f }, { 128.f, 0.f }, 7, 0.1f);
	m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f,  256.f }, { 128.f, 128.f }, { 128.f, 0.f }, 13, 0.08f);
	m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f,  384.f }, { 128.f, 128.f }, { 128.f, 0.f }, 7, 0.2f);

	m_idleState = new MirrorBossIdleState(this);
	m_chargeState = new MirrorBossChargeState(this);
	m_attackState = new MirrorBossAttackState(this);

	if (Collider* col = GetComponent<Collider>())
	{
		col->SetSize(Vec2(100.f, 100.f));
	}
	if (Rigidbody* rb = GetComponent<Rigidbody>())
	{
		rb->SetMass(10.f);
	}

	m_stateMachine->ChangeState(m_idleState);
}

MirrorBoss::~MirrorBoss()
{
	SAFE_DELETE(m_idleState);
	SAFE_DELETE(m_chargeState);
	SAFE_DELETE(m_attackState);
}

void MirrorBoss::Update()
{
	Entity::Update();
	
	UpdateTargetToPlayer();
	UpdateBossFSM();
}

void MirrorBoss::Render(HDC _hdc)
{
	BaseEnemy::Render(_hdc);
	RenderHpUI(_hdc);
	
	if (m_isCharging)
	{
		Vec2 pos = GetPos();
		GDISelector pen( _hdc, PenType::RED);
		GDISelector brush(_hdc, BrushType::HOLLOW);
		::Ellipse(_hdc, pos.x - 60, pos.y - 60, pos.x + 60, pos.y + 60);
	}
}

void MirrorBoss::OnHit(int damage)
{
	if (m_isCharging)
	{
		m_accumulatedDamage += damage;
		GET_SINGLE(EffectManager)->CreateEffect<EnemyHitEffect>(GetPos(), { 50.f,50.f }, 0.5f);
	}
	else
	{
		int reducedDamage = damage / 5; 
		if (reducedDamage < 1) reducedDamage = 1;
		
		GET_SINGLE(EffectManager)->CreateEffect<EnemyHitEffect>(GetPos(), { 50.f,50.f }, 0.5f);
		TakeDamage(reducedDamage);
	}
}

void MirrorBoss::StartCharge()
{
	m_isCharging = true;
	m_accumulatedDamage = 0;
}

void MirrorBoss::EndCharge()
{
	m_isCharging = false;
}

void MirrorBoss::FireRandomBullet()
{
	Player* target = GetTargetPlayer();
	if (!target) return;

	Vec2 myPos = GetPos();
	Vec2 targetPos = target->GetPos();
	Vec2 dir = targetPos - myPos;
	dir.Normalize();

	int totalDamage = GetAttackPower() + (m_accumulatedDamage / 2);

	int randType = rand() % (int)BossAttackType::MAX_TYPES;
	BossAttackType bulletType = static_cast<BossAttackType>(randType);

	switch (bulletType)
	{
		case BossAttackType::FAST:
		{
			FastBullet* bullet = new FastBullet();
			bullet->SetPos(myPos);
			bullet->SetDirection(dir);
			bullet->SetAttackPower(totalDamage);
			GET_SINGLE(SceneManager)->GetCurScene()->AddObject(bullet, Layer::ENEMYBULLET);
			break;
		}
		case BossAttackType::HOMING:
		{
			HomingBullet* bullet = new HomingBullet();
			bullet->SetPos(myPos);
			bullet->SetDirection(dir);
			bullet->SetTarget(target);
			bullet->SetAttackPower(totalDamage * 0.8f);
			GET_SINGLE(SceneManager)->GetCurScene()->AddObject(bullet, Layer::ENEMYBULLET);
			break;
		}
		case BossAttackType::BOOMERANG:
		{
			BoomerangBullet* bullet = new BoomerangBullet();
			bullet->SetPos(myPos);
			bullet->SetDirection(dir);
			bullet->SetAttackPower(totalDamage * 1.2f);
			GET_SINGLE(SceneManager)->GetCurScene()->AddObject(bullet, Layer::ENEMYBULLET);
			break;
		}
		case BossAttackType::FRAGMENTING:
		{
			FragmentingBullet* bullet = new FragmentingBullet();
			bullet->SetPos(myPos);
			bullet->SetDirection(dir);
			bullet->SetAttackPower(totalDamage * 0.5f);
			GET_SINGLE(SceneManager)->GetCurScene()->AddObject(bullet, Layer::ENEMYBULLET);
			break;
		}
	}
}

void MirrorBoss::UpdateBossFSM()
{
	if (m_hp <= 0 || GetIsDead())
	{
		return;
	}

	m_stateMachine->Update();

	State* curState = m_stateMachine->GetCurState();

	if (curState == m_attackState)
	{
		m_attackStateTimer += fDT;

		if (m_bulletsFiredInVolley < m_volleyCount)
		{
			m_timeSinceLastShot += fDT;
			float fireInterval = 1.f / m_attackSpeed;

			if (m_timeSinceLastShot >= fireInterval)
			{
				FireRandomBullet();
				m_bulletsFiredInVolley++;
				m_timeSinceLastShot = 0.f;
			}
		}
		
		if (m_bulletsFiredInVolley >= m_volleyCount && m_attackStateTimer >= m_attackStateDuration)
		{
			m_stateMachine->ChangeState(m_idleState);
		}
		return;
	}

	if (curState == m_idleState)
	{
		m_patternTimer += fDT;
		if (m_patternTimer >= m_idleDuration)
		{
			m_patternTimer = 0.f;
			m_stateMachine->ChangeState(m_chargeState);
		}
	}
	else if (curState == m_chargeState)
	{
		m_patternTimer += fDT;
		if (m_patternTimer >= m_chargeDuration)
		{
			m_patternTimer = 0.f;
			
			m_attackStateTimer = 0.f;
			m_bulletsFiredInVolley = 0;
			m_timeSinceLastShot = 0.f;
			
			m_stateMachine->ChangeState(m_attackState);
		}
	}
}

State* MirrorBoss::GetIdleState() const { return m_idleState; }
State* MirrorBoss::GetChargeState() const { return m_chargeState; }
State* MirrorBoss::GetAttackState() const { return m_attackState; }

void MirrorBoss::RenderHpUI(HDC _hdc)
{
	float ratio = 0.f;
	if (m_maxHp > 0)
	{
		ratio = static_cast<float>(m_hp) / static_cast<float>(m_maxHp);
		if (ratio < 0.f) ratio = 0.f;
		if (ratio > 1.f) ratio = 1.f;
	}

	int barWidth = 600;
	int barHeight = 20;
	int top = 50; 
	int left = (WINDOW_WIDTH - barWidth) / 2;
	int right = left + barWidth;
	int bottom = top + barHeight;

	HPEN oldPen = (HPEN)::SelectObject(_hdc, ::GetStockObject(BLACK_PEN));
	HBRUSH oldBrush = (HBRUSH)::SelectObject(_hdc, ::GetStockObject(BLACK_BRUSH));
	::Rectangle(_hdc, left, top, right, bottom);

	int innerLeft = left + 1;
	int innerTop = top + 1;
	int innerRight = right - 1;
	int innerBottom = bottom - 1;
	int innerWidth = innerRight - innerLeft;

	int filledWidth = static_cast<int>(innerWidth * ratio);
	int filledRight = innerLeft + filledWidth;

	if (filledWidth > 0)
	{
		GDISelector colorBrush(_hdc, BrushType::BLUE); 
		::Rectangle(_hdc, innerLeft, innerTop, filledRight, innerBottom);
	}
	
	if (filledRight < innerRight)
	{
		HBRUSH whiteBrush = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		RECT emptyRect = { filledRight, innerTop, innerRight, innerBottom };
		::FillRect(_hdc, &emptyRect, whiteBrush);
	}

	::SelectObject(_hdc, oldBrush);
	::SelectObject(_hdc, oldPen);
}
