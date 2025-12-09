#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "EffectManager.h"
#include "Scene.h"
#include "Texture.h"
#include "Collider.h"
#include "Animator.h"
#include "Animation.h"
#include "Rigidbody.h"
#include "GDISelector.h"
#include "Defines.h"
#include "PlayerIdleState.h"
#include "PlayerMoveState.h"
#include "PlayerRollingState.h"
#include "PlayerDeadState.h"
#include "BaseEnemy.h"
#include "PlayerBullet.h"
#include "PlayerAttackEffect.h"
#include "PlayerHitEffect.h"
#include "PlayerLevelUpEffect.h"
#include "PlayerRollingEffect.h"

#include <cmath>

Player::Player()
	: m_pTex(nullptr)
	, m_animator(nullptr)
	, m_rigidCompo(nullptr)
	, m_moveDirection({ 0.f, 0.f })
	, m_dashPower(700.f)
	, m_curTime(0.f)
	, m_canRolling(true)
	, m_rollingStateRemainTime(0.f)
	, m_rollingCooltime(1.5f)
	, m_canAttack(true)
	, m_attackElapsedTime(0.f)
	, m_level(1)
	, m_curExp(0)
	, m_needExp(100)
{
	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Player");
	AddComponent<Collider>()->SetSize({ 50.f,50.f });
	AddComponent<Rigidbody>();
	m_animator = AddComponent<Animator>();

	m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 10.f }, { 64.f, 64.f }, { 64.f, 0.f }, 6, 0.1f);
	m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 89.f }, { 64.f, 64.f }, { 64.f, 0.f }, 3, 0.1f);
	m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 168.f }, { 64.f, 64.f }, { 64.f, 0.f }, 7, 0.1f);
	m_animator->CreateAnimation(L"Rolling", m_pTex, { 0.f, 247.f }, { 64.f, 64.f }, { 64.f, 0.f }, 9, 0.06f);

	m_animator->Play(L"Idle");

	m_rigidCompo = GetComponent<Rigidbody>();

	m_idleState = new PlayerIdleState(this);
	m_moveState = new PlayerMoveState(this);
	m_rollingState = new PlayerRollingState(this);
	m_deadState = new PlayerDeadState(this);

	m_stateMachine->ChangeState(m_idleState);

	SetHp(10);
	m_maxHp = GetHp();
	SetMoveSpeed(200.0f);
	SetAttackCooltime(0.7f);
	SetAttackPower(5);
}

Player::~Player()
{
	SAFE_DELETE(m_idleState);
	SAFE_DELETE(m_moveState);
	SAFE_DELETE(m_rollingState);
	SAFE_DELETE(m_deadState);
	SAFE_DELETE(m_stateMachine);
}

void Player::Render(HDC _hdc)
{
	ComponentRender(_hdc);
	RenderHpUI(_hdc);
	RenderExpUI(_hdc);
}

void Player::RenderExpUI(HDC _hdc)
{
	float ratio = 0.f;
	if (m_needExp > 0)
	{
		ratio = static_cast<float>(m_curExp) / static_cast<float>(m_needExp);
		if (ratio < 0.f) ratio = 0.f;
		if (ratio > 1.f) ratio = 1.f;
	}

	int barHeight = 15;
	int barMarginX = 10;
	int barMarginBottom = 5;

	int left = barMarginX;
	int right = WINDOW_WIDTH - barMarginX;
	int bottom = WINDOW_HEIGHT - barMarginBottom;
	int top = bottom - barHeight;

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
		GDISelector greenBrush(_hdc, BrushType::GREEN);
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
}

void Player::RenderHpUI(HDC _hdc)
{
	float ratio = 0.f;
	if (m_maxHp > 0)
	{
		ratio = static_cast<float>(m_hp) / static_cast<float>(m_maxHp);
		if (ratio < 0.f) ratio = 0.f;
		if (ratio > 1.f) ratio = 1.f;
	}

	int barHeight = 15;
	int barMarginX = 10;
	int gapFromExp = 10;

	int expBarHeight = 15;
	int expBarMarginBottom = 5;

	int expBottom = WINDOW_HEIGHT - expBarMarginBottom;
	int expTop = expBottom - expBarHeight;

	int bottom = expTop - gapFromExp;
	int top = bottom - barHeight;

	int left = barMarginX;
	int right = left + 300;

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
	CooldownAttackTime();

	if (m_rollingStateRemainTime > 0.f)
	{
		m_rollingStateRemainTime -= fDT;
		if (m_rollingStateRemainTime < 0.f)
			m_rollingStateRemainTime = 0.f;
	}

	BlockPlayer();
}

void Player::UpdateInput()
{
	if (m_rigidCompo == nullptr || m_isDeadState)
		return;

	POINT mousePt = GET_MOUSEPOS;
	Vec2 mousePos((float)mousePt.x, (float)mousePt.y);
	Vec2 playerPos = GetPos();

	bool flipX = (mousePos.x < playerPos.x);
	m_animator->SetFlipX(flipX);

	m_moveDirection = { 0.f, 0.f };
	if (GET_KEY(KEY_TYPE::A)) m_moveDirection.x -= 1.f;
	if (GET_KEY(KEY_TYPE::D)) m_moveDirection.x += 1.f;
	if (GET_KEY(KEY_TYPE::W)) m_moveDirection.y -= 1.f;
	if (GET_KEY(KEY_TYPE::S)) m_moveDirection.y += 1.f;

	bool hasInput = m_moveDirection != Vec2{ 0.f, 0.f };

	if (GET_KEYDOWN(KEY_TYPE::SPACE) && m_canAttack)
	{
		Attack();
	}

	if (m_rollingStateRemainTime > 0.f)
		return;

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

	if (GET_KEYDOWN(KEY_TYPE::LSHIFT) && hasInput && m_canRolling)
	{
		m_stateMachine->ChangeState(m_rollingState);
		m_canRolling = false;
	}
}


void Player::CooldownRollingTime()
{
	if (!m_canRolling)
		m_curTime += fDT;

	if (m_curTime >= m_rollingCooltime)
	{
		m_canRolling = true;
		m_curTime = 0.f;
	}
}

void Player::CooldownAttackTime()
{
	if (!m_canAttack)
	{
		m_attackElapsedTime += fDT;
		if (m_attackElapsedTime >= m_attackCooltime)
		{
			m_attackElapsedTime = 0.f;
			m_canAttack = true;
		}
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
		m_rigidCompo->SetVelocity({ 0.f, 0.f });
}

void Player::TakeDamage(int _damage)
{
	if (m_isInvincibility == true) return;
	GET_SINGLE(EffectManager)->CreateEffect<PlayerHitEffect>(GetPos(), { 50.f,50.f }, 2.f);
	Entity::TakeDamage(_damage);
}

void Player::Rolling()
{
	m_rigidCompo->AddImpulse(m_moveDirection * m_dashPower);
	GET_SINGLE(EffectManager)->CreateEffect<PlayerRollingEffect>(GetPos(), { 50.f,50.f }, 2.f);
	m_rollingStateRemainTime = 0.6f;
}

void Player::Attack()
{
	std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
	if (curScene != nullptr)
	{
		POINT mousePt = GET_MOUSEPOS;
		Vec2 mousePos((float)mousePt.x, (float)mousePt.y);

		Vec2 playerPos = GetPos();

		Vec2 dir{ mousePos.x - playerPos.x, mousePos.y - playerPos.y };
		float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
		if (len > 0.f)
		{
			dir.x /= len;
			dir.y /= len;
		}
		else
		{
			dir = Vec2{ 1.f, 0.f };
		}

		Vec2 bulletSize{ 30.f, 30.f };
		Vec2 bulletPos = playerPos + dir * (GetSize().x * 0.5f + bulletSize.x * 0.5f);

		PlayerBullet* bullet = new PlayerBullet;
		bullet->SetPos(bulletPos);
		bullet->SetSize(bulletSize);
		bullet->SetDirection(dir);
		bullet->SetAttackPower(GetAttackPower());

		curScene->AddObject(bullet, Layer::PLAYERBULLET);

		GET_SINGLE(EffectManager)->CreateEffect<PlayerAttackEffect>(GetPos(), { 50.f,50.f }, 2.f);

		m_canAttack = false;
		m_attackElapsedTime = 0.f;
	}
}

void Player::Dead()
{
	m_isDeadState = true;
	if (m_stateMachine && m_deadState)
	{
		m_stateMachine->ChangeState(m_deadState);
	}
}

void Player::Destroy()
{
	std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
	if (curScene)
	{
		curScene->RequestDestroy(this);
	}
}

void Player::Move()
{
	Translate({ fDT * m_moveDirection.x * m_moveSpeed,
				fDT * m_moveDirection.y * m_moveSpeed });
}

void Player::ChangeAnimation(wstring animationName, bool isLoop)
{
	m_animator->Play(animationName, isLoop ? PlayMode::Loop : PlayMode::Once);
}

int Player::LevelUp(int level)
{
	GET_SINGLE(EffectManager)->CreateEffect<PlayerLevelUpEffect>(GetPos(), { 70.f,70.f }, 2.f);
	m_rollingCooltime -= 0.1f;
	SetAttackCooltime(GetAttackCooltime() - 0.05f);
	SetAttackPower(GetAttackPower() + 1);
	m_maxHp += 1;
	SetHp(m_maxHp);
	return (int)(m_needExp * 1.5f);
}

void Player::TakeExp(int exp)
{
	m_curExp += exp;
	if (m_needExp <= m_curExp)
	{
		m_curExp = 0;
		m_level++;
		m_needExp = LevelUp(m_level);
	}
}
