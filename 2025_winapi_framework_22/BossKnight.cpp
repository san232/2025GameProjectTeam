#include "pch.h"
#include "BossKnight.h"
#include "BossChargeState.h"
#include "BossDashState.h"
#include "EnemyIdleState.h"
#include "EnemyMoveState.h"
#include "EnemyAttackState.h"
#include "EnemyHitState.h"
#include "EnemyDeadState.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Animator.h"
#include "GDISelector.h"
#include "Player.h"
#include "SceneManager.h"
#include "Scene.h"
#include "EffectManager.h"
#include "EnemyHitEffect.h"
#include "BossBullet.h"
#include "Texture.h"

BossKnight::BossKnight()
	: m_chargeState(nullptr)
	, m_dashState(nullptr)
	, m_patternCooldown(1.0f)
	, m_patternTimer(0.f)
	, m_chargeTimer(0.f)
	, m_chargeDuration(0.5f)
	, m_dashTimer(0.f)
	, m_dashDuration(0.8f)
	, m_hasAttacked(false)
	, m_maxHp(150)
{
	SetHp(m_maxHp);
	SetMoveSpeed(300.f);
	SetAttackPower(3.f);
	SetAttackCooltime(1.5f);
	SetAttackRange(120.f); 
	SetExp(500);
	SetDefaultLookRight(true);
	SetStatMulti();
	m_maxHp = GetHp();
	m_hitStunDuration = 0.1f;
	
	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"DashBoss"); 
	m_hpBarTex = GET_SINGLE(ResourceManager)->GetTexture(L"DashBossHpBar");

	m_animator->SetScaleRatio(2.5f);

	m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 0.f }, { 128.f, 128.f }, { 128.f,0.f }, 4, 0.1f);
	m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 128.f }, { 128.f, 128.f }, { 128.f,0.f }, 4, 0.1f);
	m_animator->CreateAnimation(L"Attack", m_pTex, { 0.f, 640.f }, { 128.f, 128.f }, { 128.f,0.f }, 11, 0.1f);
	m_animator->CreateAnimation(L"Hit", m_pTex, { 0.f, 256.f }, { 128.f, 128.f }, { 128.f,  0.f }, 2, 0.04f);
	m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 256.f }, { 128.f, 128.f }, { 128.f, 0.f }, 9, 0.1f);
	
	m_animator->CreateAnimation(L"Charge", m_pTex, { 0.f, 384.f }, { 128.f, 128.f }, { 128.f,0.f }, 4, 0.1f); 
	m_animator->CreateAnimation(L"Dash", m_pTex, { 0.f, 768.f }, { 128.f, 128.f }, { 128.f,0.f }, 4, 0.1f);

	m_chargeState = new BossChargeState(this);
	m_dashState = new BossDashState(this);

	m_attackStateDuration = 1.1f;

	if (Collider* col = GetComponent<Collider>())
	{
		col->SetSize(Vec2(120.f, 160.f)); 
	}
	if (Rigidbody* rb = GetComponent<Rigidbody>())
	{
		rb->SetMass(5.0f);
	}
}

BossKnight::~BossKnight()
{
	SAFE_DELETE(m_chargeState);
	SAFE_DELETE(m_dashState);
}

void BossKnight::Update()
{
	Entity::Update();
	m_position = GetPos();
	m_targetPlayer = FindPlayer();

	if (m_targetPlayer)
		SetTargetPosition(m_targetPlayer->GetPos());

	UpdateAttackCooldown();
	UpdateBossFSM();

	if (m_stateMachine->GetCurState() != m_dashState && m_stateMachine->GetCurState() != m_chargeState)
	{
		UpdateFlipToTarget();
	}
}

void BossKnight::Render(HDC _hdc)
{
	BaseEnemy::Render(_hdc);
	RenderBossHpUI(_hdc);
}

void BossKnight::EnterCollision(Collider* _other)
{
}

void BossKnight::StayCollision(Collider* _other)
{
	if (m_stateMachine->GetCurState() == m_dashState)
	{
		Object* obj = _other->GetOwner();
		Entity* target = dynamic_cast<Entity*>(obj);

		if (target && !target->GetIsDead())
		{
            if (m_dashHitEntities.find(target) != m_dashHitEntities.end())
                return;

            m_dashHitEntities.insert(target);

			target->TakeDamage(GetAttackPower() * 0.5f);
			
			Vec2 bossPos = GetPos();
			Vec2 targetPos = target->GetPos();
			Vec2 pushDir = targetPos - bossPos;
			pushDir.Normalize();
			
			if (Rigidbody* trb = target->GetComponent<Rigidbody>())
			{
				trb->AddImpulse(pushDir * 800.f); 
			}
		}
	}
}

void BossKnight::ExitCollision(Collider* _other)
{
}

State* BossKnight::GetIdleState() const
{
	return m_idleState;
}

State* BossKnight::GetMoveState() const
{
	return m_moveState;
}

State* BossKnight::GetAttackState() const
{
	return m_attackState;
}

void BossKnight::Attack()
{
}

void BossKnight::ApplyDamage()
{
	Player* player = GetTargetPlayer();
	if (!player || player->GetIsDead())
		return;

	Vec2 bossPos = GetPos();
	Vec2 playerPos = player->GetPos();

	float dist = (bossPos - playerPos).Length();
	if (dist < m_attackRange) 
	{
		player->TakeDamage(GetAttackPower());

		Vec2 pushDir = playerPos - bossPos;
		pushDir.Normalize();

		if (Rigidbody* rb = player->GetComponent<Rigidbody>())
		{
			rb->AddImpulse(pushDir * 800.f);
		}
	}
	Fire8DirectionBullets();
}

void BossKnight::Fire8DirectionBullets()
{
	std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
	if (!curScene) return;

	Vec2 bossPos = GetPos();
	Vec2 offsets[] = {
		{1.f, 0.f}, {0.7071f, 0.7071f}, {0.f, 1.f}, {-0.7071f, 0.7071f},
		{-1.f, 0.f}, {-0.7071f, -0.7071f}, {0.f, -1.f}, {0.7071f, -0.7071f}
	};

	for (int i = 0; i < 8; ++i)
	{
		BossBullet* bullet = new BossBullet;
		bullet->SetPos(bossPos);
		bullet->SetSize(Vec2(20.f, 20.f));
		bullet->SetDirection(offsets[i]);
		curScene->AddObject(bullet, Layer::ENEMYBULLET);
	}
}

void BossKnight::Dead()
{
	BaseEnemy::Dead();
}

void BossKnight::OnHit(int damage)
{
	if (m_stateMachine->GetCurState() == m_dashState || m_stateMachine->GetCurState() == m_chargeState)
	{
		GET_SINGLE(EffectManager)->CreateEffect<EnemyHitEffect>(GetPos(), { 50.f,50.f }, 2.f);
		Entity::TakeDamage(damage);

		if (m_hp <= 0 || GetIsDead())
		{
			if (m_stateMachine && m_deadState)
			{
				m_stateMachine->ChangeState(m_deadState);
			}
			return;
		}
	}
	else
	{
		BaseEnemy::OnHit(damage);
	}
}

void BossKnight::UpdateBossFSM()
{
	if (m_hp <= 0 || GetIsDead())
	{
		if (m_stateMachine && m_deadState)
		{
			m_stateMachine->ChangeState(m_deadState);
		}
		if (m_stateMachine)
			m_stateMachine->Update();
		return;
	}

	if (m_inHitStun)
	{
		m_hitStunTimer += fDT;
		if (m_hitStunTimer >= m_hitStunDuration)
		{
			m_inHitStun = false;
		}
		else
		{
			if (m_stateMachine && m_hitState)
			{
				m_stateMachine->ChangeState(m_hitState);
				m_stateMachine->Update();
				return;
			}
		}
	}

	State* curState = m_stateMachine->GetCurState();
	if (curState == m_chargeState)
	{
		m_chargeTimer += fDT;
		if (m_chargeTimer >= m_chargeDuration)
		{
			m_dashTimer = 0.f;
			m_stateMachine->ChangeState(m_dashState);
		}
		m_stateMachine->Update();
		return;
	}
	
	if (curState == m_dashState)
	{
		m_stateMachine->Update();
		return;
	}

	if (curState == m_attackState)
	{
		m_attackStateTimer += fDT;
		if (m_attackStateTimer >= 0.6f && !m_hasAttacked)
		{
			ApplyDamage();
			m_hasAttacked = true;
		}

		if (m_attackStateTimer >= m_attackStateDuration)
		{
			m_stateMachine->ChangeState(m_idleState);
			m_attackStateTimer = 0.f;
		}
		else
		{
			m_stateMachine->Update();
		}
		return;
	}

	m_patternTimer += fDT;

	if (!m_targetPlayer || m_targetPlayer->GetIsDead())
	{
		if (m_stateMachine && m_idleState)
		{
			m_stateMachine->ChangeState(m_idleState);
			m_stateMachine->Update();
		}
		return;
	}

	float dist = (m_targetPosition - m_position).Length();
	
	if (m_patternTimer >= m_patternCooldown)
	{
		m_patternTimer = 0.f;
		m_chargeTimer = 0.f;
		m_stateMachine->ChangeState(m_chargeState); 
		m_stateMachine->Update();
		return;
	}

	if (dist <= m_attackRange && m_canAttack)
	{
		m_canAttack = false;
		m_attackStateTimer = 0.f;
		m_hasAttacked = false;
		m_stateMachine->ChangeState(m_attackState);
		m_stateMachine->Update();
	}
	else
	{
		if (m_stateMachine && m_moveState)
		{
			m_stateMachine->ChangeState(m_moveState);
		}
		m_stateMachine->Update();
	}
}

void BossKnight::RenderBossHpUI(HDC _hdc)
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
	int barMarginBottom = 50; 

	int left = (WINDOW_WIDTH - barWidth) / 2;
	int right = left + barWidth;
	int top = 50; 
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
		GDISelector redBrush(_hdc, BrushType::RED);
		::Rectangle(_hdc, innerLeft, innerTop, filledRight, innerBottom);
	}

	if (filledRight < innerRight)
	{
		HBRUSH whiteBrush = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		HBRUSH prevBrush = (HBRUSH)::SelectObject(_hdc, whiteBrush);
		::Rectangle(_hdc, filledRight, innerTop, innerRight, innerBottom);
		::SelectObject(_hdc, prevBrush);
	}

	::SelectObject(_hdc, oldBrush);
	::SelectObject(_hdc, oldPen);

	HDC texDC = m_hpBarTex->GetTextureDC();

	::TransparentBlt(_hdc, left
		, top - 20
		, barWidth
		, barHeight + 40
		, texDC
		, 0
		, 0
		, m_hpBarTex->GetWidth()
		, m_hpBarTex->GetHeight()
		, RGB(255,0,255));
}

