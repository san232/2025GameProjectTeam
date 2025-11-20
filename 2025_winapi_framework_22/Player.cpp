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
Player::Player()
{

}

Player::~Player()
{
	// DELETE
}
void Player::Render(HDC _hdc)
{
	Vec2 pos = GetPos();
	Vec2 size = GetSize();
	LONG width = m_pTex->GetWidth();
	LONG height = m_pTex->GetHeight();

	ComponentRender(_hdc);
}

void Player::StayCollision(Collider* _other)
{
}

void Player::EnterCollision(Collider* _other)
{
}


void Player::ExitCollision(Collider* _other)
{
}


void Player::Update()
{
	
}

