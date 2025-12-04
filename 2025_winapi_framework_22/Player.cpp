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
#include "PlayerRollingState.h"
#include "PlayerDeadState.h"
#include "BaseEnemy.h"
#include "PlayerBullet.h"

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
    , m_rollingCooltime(0.5f)
    , m_canAttack(true)
    , m_attackElapsedTime(0.f)
    , m_level(1)
    , m_curExp(0)
    , m_needExp(50)
{
    m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Jiwoo");
    AddComponent<Collider>();
    AddComponent<Rigidbody>();
    m_animator = AddComponent<Animator>();

    m_animator->CreateAnimation(L"Idle", m_pTex, { 0.f, 150.f }, { 50.f, 50.f }, { 50.f, 0.f }, 5, 0.1f);
    m_animator->CreateAnimation(L"Move", m_pTex, { 0.f, 100.f }, { 50.f, 50.f }, { 50.f, 0.f }, 5, 0.1f);
    m_animator->CreateAnimation(L"Rolling", m_pTex, { 0.f, 200.f }, { 50.f, 50.f }, { 50.f, 0.f }, 5, 0.1f);
    m_animator->CreateAnimation(L"Dead", m_pTex, { 0.f, 0.f }, { 50.f, 50.f }, { 50.f, 0.f }, 5, 0.1f);

    m_animator->Play(L"Idle");

    m_rigidCompo = GetComponent<Rigidbody>();

    m_idleState = new PlayerIdleState(this);
    m_moveState = new PlayerMoveState(this);
    m_rollingState = new PlayerRollingState(this);
    m_deadState = new PlayerDeadState(this);

    m_stateMachine->ChangeState(m_idleState);

    SetHp(10);
    SetMoveSpeed(200.0f);
    SetAttackCooltime(0.2f);
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
    if (m_rigidCompo == nullptr)
        return;

    m_moveDirection = { 0.f, 0.f };
    if (GET_KEY(KEY_TYPE::A)) m_moveDirection.x -= 1.f;
    if (GET_KEY(KEY_TYPE::D)) m_moveDirection.x += 1.f;
    if (GET_KEY(KEY_TYPE::W)) m_moveDirection.y -= 1.f;
    if (GET_KEY(KEY_TYPE::S)) m_moveDirection.y += 1.f;

    bool hasInput = m_moveDirection != Vec2{ 0.f, 0.f };

    if (GET_KEYDOWN(KEY_TYPE::SPACE) && m_canAttack)
    {
        std::shared_ptr<Scene> curScene = GET_SINGLE(SceneManager)->GetCurScene();
        if (curScene)
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

            Vec2 bulletSize{ 12.f, 12.f };
            Vec2 bulletPos = playerPos + dir * (GetSize().x * 0.5f + bulletSize.x * 0.5f);

            PlayerBullet* bullet = new PlayerBullet;
            bullet->SetPos(bulletPos);
            bullet->SetSize(bulletSize);
            bullet->SetDirection(dir);

            curScene->AddObject(bullet, Layer::PLAYERBULLET);

            m_canAttack = false;
            m_attackElapsedTime = 0.f;
        }
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

void Player::Attack()
{
    if (m_rigidCompo)
        m_rigidCompo->AddImpulse(m_moveDirection * m_dashPower);

    m_rollingStateRemainTime = 0.3f;
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

void Player::ChangeAnimation(wstring animationName)
{
    m_animator->Play(animationName);
}

int Player::LevelUp(int level)
{
    cout << "LevelUp";
    return (int)(m_needExp * 1.5f);
}

void Player::TakeExp(int exp)
{
    m_curExp += exp;
    if (m_needExp <= m_curExp)
    {
        m_curExp -= m_needExp;
        m_level++;
        m_needExp = LevelUp(m_level);
    }
}
