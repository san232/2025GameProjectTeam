// Player.cpp
#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "Animator.h"
#include "Animation.h"
#include "Rigidbody.h"
#include "GDISelector.h"
#include "Defines.h"
#include "PlayerIdleState.h"
#include "PlayerMoveState.h"
#include "PlayerAttackState.h"
#include "PlayerDeadState.h"
#include "BaseEnemy.h"

#include <cmath>

Player::Player()
	: m_pTex(nullptr)
	, m_animator(nullptr)
	, m_rigidCompo(nullptr)
	, m_moveDirection({ 0.f,0.f })
	, m_dashPower(700.f)
	, m_curTime(0.f)
	, m_isCanAttack(true)
	, m_attackDelayTimer(0.f)
	, m_attackPending(false)
	, m_attackDelay(0.5f)
	, m_attackSize(1.2f)
	, m_attackStateRemainTime(0.3f)
{
	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Jiwoo");
	AddComponent<Collider>();
	AddComponent<Rigidbody>();
	m_animator = AddComponent<Animator>();

	m_animator->CreateAnimation (L"Idle", m_pTex, { 0.f,150.f }, { 50.f,50.f }, { 50.f,0.f }, 5, 0.1f);
	m_animator->CreateAnimation (L"Move", m_pTex, { 0.f,100.f }, { 50.f,50.f }, { 50.f,0.f }, 5, 0.1f);
	m_animator->CreateAnimation (L"Attack", m_pTex, { 0.f,200.f }, { 50.f,50.f }, { 50.f,0.f }, 5, 0.1f);
	m_animator->CreateAnimation (L"Dead", m_pTex, { 0.f,0.f }, { 50.f,50.f }, { 50.f,0.f }, 5, 0.1f);

	m_animator->Play(L"Idle");

	m_rigidCompo = GetComponent<Rigidbody>();

	m_idleState = new PlayerIdleState(this);
	m_moveState = new PlayerMoveState(this);
	m_attackState = new PlayerAttackState(this);
	m_deadState = new PlayerDeadState(this);

	m_stateMachine->ChangeState(m_idleState);

	SetHp(10);
	SetMoveSpeed(200.0f);
	SetAttackCooltime(0.5f);
	SetAttackPower(2);
}

Player::~Player()
{
	SAFE_DELETE(m_idleState);
	SAFE_DELETE(m_moveState);
	SAFE_DELETE(m_attackState);
	SAFE_DELETE(m_deadState);
	SAFE_DELETE(m_stateMachine);
}

void Player::Render(HDC _hdc)
{
	Vec2 pos = GetPos();
	Vec2 size = GetSize();

	BrushType brush = BrushType::HOLLOW;
	PenType   pen = PenType::RED;

	GDISelector brushSelector(_hdc, brush);
	GDISelector penSelector(_hdc, pen);

	Vec2 attackSize = { size.x * m_attackSize,
					 size.y * m_attackSize };

	if (m_attackPending)
	{
		RECT_RENDER(_hdc, pos.x, pos.y, attackSize.x, attackSize.y);
	}

	ComponentRender(_hdc);
}

void Player::EnterCollision(Collider* _other)
{
}

void Player::StayCollision(Collider* _other)
{
}

void Player::ExitCollision(Collider* _other)
{
}

void Player::Update()
{
	Entity::Update();
	m_stateMachine->Update();

	UpdateInput();
	CooldownRollingTime();
	UpdateAttackDelay();

	if (m_attackStateRemainTime > 0.f)
	{
		m_attackStateRemainTime -= fDT;
		if (m_attackStateRemainTime < 0.f)
			m_attackStateRemainTime = 0.f;
	}

	BlockPlayer();
}



void Player::UpdateInput()
{
	if (m_rigidCompo == nullptr)
		return;

	m_moveDirection = { 0.f,0.f };
	if (GET_KEY(KEY_TYPE::A)) m_moveDirection.x -= 1.f;
	if (GET_KEY(KEY_TYPE::D)) m_moveDirection.x += 1.f;
	if (GET_KEY(KEY_TYPE::W)) m_moveDirection.y -= 1.f;
	if (GET_KEY(KEY_TYPE::S)) m_moveDirection.y += 1.f;

	bool hasInput = m_moveDirection != Vec2{ 0.f,0.f };

	if (m_attackStateRemainTime > 0.f)
	{
		return;
	}

	if (hasInput)
	{
		float len = sqrtf(m_moveDirection.x * m_moveDirection.x + m_moveDirection.y * m_moveDirection.y);
		if (len > 0.f)
		{
			m_moveDirection.x /= len;
			m_moveDirection.y /= len;
		}
		m_stateMachine->ChangeState(m_moveState);
	}
	else
	{
		m_stateMachine->ChangeState(m_idleState);
	}

	if (GET_KEYDOWN(KEY_TYPE::SPACE) && hasInput && m_isCanAttack)
	{
		m_stateMachine->ChangeState(m_attackState);
		m_isCanAttack = false;
	}
}



void Player::CooldownRollingTime()
{
	if (!m_isCanAttack)
		m_curTime += fDT;

	if (m_curTime >= m_attackCooltime)
	{
		m_isCanAttack = true;
		m_curTime = 0.f;
	}
}

void Player::UpdateAttackDelay()
{
	if (!m_attackPending)
		return;

	m_attackDelayTimer += fDT;
	if (m_attackDelayTimer >= m_attackDelay)
	{
		m_attackDelayTimer = 0.f;
		m_attackPending = false;

		PerformAreaAttack();
	}
}

void Player::BlockPlayer()
{
	Vec2 pos = GetPos();
	Vec2 size = GetSize();

	float x = size.x * 0.5f;
	float y = size.y * 0.5f;

	float minX = x;
	float maxX = WINDOW_WIDTH - x;
	float minY = y;
	float maxY = WINDOW_HEIGHT - y;

	pos.x = std::clamp(pos.x, minX, maxX);
	pos.y = std::clamp(pos.y, minY, maxY);

	SetPos(pos);
}

void Player::StopMoving()
{
	if (m_rigidCompo)
		m_rigidCompo->SetVelocity({ 0.f,0.f });
}

void Player::Attack()
{
	if (m_rigidCompo)
		m_rigidCompo->AddImpulse(m_moveDirection * m_dashPower);

	m_attackDelayTimer = 0.f;
	m_attackPending = true;

	m_attackStateRemainTime = m_attackDelay;
}


void Player::Dead()
{
	cout << "플레이어 죽음" << endl;
	if (m_stateMachine && m_deadState)
	{
		m_stateMachine->ChangeState(m_deadState);
	}
}

void Player::Move()
{
	Translate({ fDT * m_moveDirection.x * m_moveSpeed,
				fDT * m_moveDirection.y * m_moveSpeed });
}

void Player::PerformAreaAttack()
{
	std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
	if (!curScene)
		return;

	Vec2 playerPos = GetPos();
	Vec2 playerSize = GetSize();

	Vec2 aoeSize = { playerSize.x * m_attackSize,
					 playerSize.y * m_attackSize };

	Vec2 aoeCenter = playerPos;

	float halfAw = aoeSize.x * 0.5f;
	float halfAh = aoeSize.y * 0.5f;

	auto ProcessEnemyLayer = [&](Layer layer)
		{
			const std::vector<Object*>& objs = curScene->GetLayerObjects(layer);

			for (Object* obj : objs)
			{
				if (!obj || obj->GetIsDead())
					continue;

				BaseEnemy* enemy = dynamic_cast<BaseEnemy*>(obj);
				if (!enemy)
					continue;

				Vec2 enemyPos = enemy->GetPos();
				Vec2 enemySize = enemy->GetSize();

				float halfEw = enemySize.x * 0.5f;
				float halfEh = enemySize.y * 0.5f;

				float dx = std::fabs(enemyPos.x - aoeCenter.x);
				float dy = std::fabs(enemyPos.y - aoeCenter.y);

				bool isHit =
					(dx <= (halfAw + halfEw)) &&
					(dy <= (halfAh + halfEh));

				if (isHit)
				{
					enemy->OnHit(GetAttackPower());
				}
			}
		};

	ProcessEnemyLayer(Layer::DEFAULTENEMY);
	ProcessEnemyLayer(Layer::INVISIBLEENEMY);
}

void Player::ChangeAnimation(wstring animationName)
{
	m_animator->Play(animationName);
}
