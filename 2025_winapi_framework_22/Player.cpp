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

Player::Player()
    : m_pTex(nullptr)
    , m_stateMachine(*this)
{
    AddComponent<Collider>();
    AddComponent<Rigidbody>();
    AddComponent<Animator>();

    m_stateMachine.AddState(PlayerStateType::Idle, &m_idleState);
    m_stateMachine.AddState(PlayerStateType::Move, &m_moveState);
    m_stateMachine.AddState(PlayerStateType::Rolling, &m_rollingState);
    m_stateMachine.AddState(PlayerStateType::Dead, &m_deadState);
    m_stateMachine.ChangeState(PlayerStateType::Idle);
}

Player::~Player()
{
}

void Player::Render(HDC _hdc)
{
    Vec2 pos = GetPos();
    Vec2 size = GetSize();

    PlayerStateType stateType = m_stateMachine.GetCurrentId();

    BrushType brush = BrushType::HOLLOW;
    PenType pen = PenType::GREEN;

    switch (stateType)
    {
    case PlayerStateType::Idle:
        brush = BrushType::GREEN;
        pen = PenType::GREEN;
        break;
    case PlayerStateType::Move:
        brush = BrushType::RED;
        pen = PenType::RED;
        break;
    case PlayerStateType::Rolling:
        brush = BrushType::GREEN;
        pen = PenType::RED;
        break;
    case PlayerStateType::Dead:
        brush = BrushType::HOLLOW;
        pen = PenType::RED;
        break;
    default:
        break;
    }

    GDISelector brushSelector(_hdc, brush);
    GDISelector penSelector(_hdc, pen);

    RECT_RENDER(_hdc, pos.x, pos.y, size.x, size.y);

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
    m_stateMachine.Update();
}

void Player::ChangeState(PlayerStateType stateType)
{
    m_stateMachine.ChangeState(stateType);
}
