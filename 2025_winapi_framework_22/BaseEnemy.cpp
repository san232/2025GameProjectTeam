#include "pch.h"
#include "BaseEnemy.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Defines.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Player.h"
#include "EnemyIdleState.h"
#include "EnemyMoveState.h"
#include "EnemyAttackState.h"
#include "EnemyHitState.h"
#include "EnemyDeadState.h"
#include "GDISelector.h"

BaseEnemy::BaseEnemy()
	: m_stateMachine(nullptr)
	, m_idleState(nullptr)
	, m_moveState(nullptr)
	, m_attackState(nullptr)
	, m_hitState(nullptr)
	, m_deadState(nullptr)
	, m_position{}
	, m_targetPosition{}
	, m_attackRange(50.f)
	, m_attackTimer(0.f)
	, m_canAttack(true)
	, m_attackDelay(0.5f)
	, m_attackDelayTimer(0.f)
	, m_isPreparingAttack(false)
	, m_targetPlayer(nullptr)
	, m_inHitStun(false)
	, m_hitStunTimer(0.f)
	, m_hitStunDuration(0.2f)
{
	AddComponent<Rigidbody>();
	AddComponent<Collider>();

	m_stateMachine = new StateMachine;

	m_idleState = new EnemyIdleState(this);
	m_moveState = new EnemyMoveState(this);
	m_attackState = new EnemyAttackState(this);
	m_hitState = new EnemyHitState(this);
	m_deadState = new EnemyDeadState(this);

	m_stateMachine->ChangeState(m_idleState);
}

BaseEnemy::~BaseEnemy()
{
	SAFE_DELETE(m_idleState);
	SAFE_DELETE(m_moveState);
	SAFE_DELETE(m_attackState);
	SAFE_DELETE(m_hitState);
	SAFE_DELETE(m_deadState);
	SAFE_DELETE(m_stateMachine);
}

void BaseEnemy::Update()
{
	Entity::Update();

	m_position = GetPos();

	UpdateTargetToPlayer();
	UpdateAttackCooldown();
	UpdateFSM();
}

void BaseEnemy::Render(HDC _hdc)
{
	Vec2 pos = GetPos();
	float rng = m_attackRange;

	GDISelector brushSelector(_hdc, BrushType::HOLLOW);
	GDISelector penSelector(_hdc, PenType::GREEN);
	ELLIPSE_RENDER(_hdc, pos.x, pos.y, rng * 2.f, rng * 2.f);

	ComponentRender(_hdc);
}

void BaseEnemy::EnterCollision(Collider* _other)
{
}

void BaseEnemy::StayCollision(Collider* _other)
{
}

void BaseEnemy::ExitCollision(Collider* _other)
{
}

void BaseEnemy::MoveToTarget()
{
	Vec2 currentPos = m_position;
	Vec2 targetPos = m_targetPosition;
	Vec2 direction = { targetPos.x - currentPos.x, targetPos.y - currentPos.y };

	if (IsInAttackRange() || (direction.x == 0.f && direction.y == 0.f))
	{
		if (Rigidbody* rigidbody = GetComponent<Rigidbody>())
		{
			rigidbody->SetVelocity({ 0.f, 0.f });
		}
		return;
	}

	float dist = sqrtf(direction.x * direction.x + direction.y * direction.y);
	if (dist > 0.f)
	{
		direction.x /= dist;
		direction.y /= dist;
	}

	Vec2 velocity = { direction.x * m_moveSpeed, direction.y * m_moveSpeed };
	if (Rigidbody* rigidbody = GetComponent<Rigidbody>())
	{
		rigidbody->SetVelocity(velocity);
	}
}

bool BaseEnemy::IsInAttackRange() const
{
	float dx = m_targetPosition.x - m_position.x;
	float dy = m_targetPosition.y - m_position.y;
	float distanceSq = dx * dx + dy * dy;

	return distanceSq <= (m_attackRange * m_attackRange);
}

void BaseEnemy::OnHit(int damage)
{
	Entity::TakeDamage(damage);

	if (m_hp <= 0 || GetIsDead())
	{
		if (m_stateMachine && m_deadState)
		{
			m_stateMachine->ChangeState(m_deadState);
		}
		return;
	}

	m_inHitStun = true;
	m_hitStunTimer = 0.f;

	if (m_stateMachine && m_hitState)
	{
		m_stateMachine->ChangeState(m_hitState);
	}
}

void BaseEnemy::Dead()
{
	m_targetPlayer->TakeExp(m_exp);
	if (m_stateMachine && m_deadState)
	{
		m_stateMachine->ChangeState(m_deadState);
	}
}

void BaseEnemy::Move()
{
	MoveToTarget();
}

void BaseEnemy::UpdateFSM()
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

	if (!m_targetPlayer)
	{
		m_isPreparingAttack = false;
		m_attackDelayTimer = 0.f;

		if (m_stateMachine && m_idleState)
		{
			m_stateMachine->ChangeState(m_idleState);
			m_stateMachine->Update();
		}
		return;
	}

	const bool inRange = IsInAttackRange();

	if (inRange)
	{
		if (m_canAttack)
		{
			if (!m_isPreparingAttack)
			{
				m_isPreparingAttack = true;
				m_attackDelayTimer = 0.f;
			}
			else
			{
				m_attackDelayTimer += fDT;
			}

			if (m_attackDelayTimer >= m_attackDelay && m_stateMachine && m_attackState)
			{
				m_isPreparingAttack = false;
				m_attackDelayTimer = 0.f;
				m_canAttack = false; 

				m_stateMachine->ChangeState(m_attackState);
				m_stateMachine->Update(); 
				return;
			}
		}
		else
		{
			m_isPreparingAttack = false;
			m_attackDelayTimer = 0.f;
		}

		if (m_stateMachine && m_idleState)
		{
			m_stateMachine->ChangeState(m_idleState);
		}
	}
	else
	{
		m_isPreparingAttack = false;
		m_attackDelayTimer = 0.f;

		if (m_stateMachine && m_moveState)
		{
			m_stateMachine->ChangeState(m_moveState);
		}
	}

	if (m_stateMachine)
		m_stateMachine->Update();
}

void BaseEnemy::UpdateAttackCooldown()
{
	if (!m_canAttack)
	{
		m_attackTimer += fDT;
		if (m_attackTimer >= m_attackCooltime)
		{
			m_attackTimer = 0.f;
			m_canAttack = true;
		}
	}
}

void BaseEnemy::UpdateTargetToPlayer()
{
	m_targetPlayer = FindPlayer();
	if (m_targetPlayer)
	{
		SetTargetPosition(m_targetPlayer->GetPos());
	}
}

Player* BaseEnemy::FindPlayer() const
{
	std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
	if (!curScene)
		return nullptr;

	const vector<Object*>& objs = curScene->GetLayerObjects(Layer::PLAYER);

	Player* player = nullptr;
	if (objs.size() > 0)
		player = dynamic_cast<Player*>(objs[0]);

	return player;
}
