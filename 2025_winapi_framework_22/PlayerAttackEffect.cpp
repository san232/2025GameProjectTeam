#include "pch.h"
#include "PlayerAttackEffect.h"
#include "Animator.h"

PlayerAttackEffect::PlayerAttackEffect()
{
	m_animator = AddComponent<Animator>();
}

PlayerAttackEffect::~PlayerAttackEffect()
{

}

void PlayerAttackEffect::Update()
{

}

void PlayerAttackEffect::Render(HDC _hdc)
{

}