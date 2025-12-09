#include "pch.h"
#include "PlayerAttackEffect.h"
#include "Animator.h"
#include "Texture.h"
#include "ResourceManager.h"

PlayerAttackEffect::PlayerAttackEffect()
{
	m_animator = AddComponent<Animator>();

	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerAttackEffect");

	m_animator->CreateAnimation(L"Effect", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 64.f, 0.f }, 4, 0.05f);
	m_animator->Play(L"Effect", PlayMode::Once);
}

PlayerAttackEffect::~PlayerAttackEffect()
{
}

void PlayerAttackEffect::Update()
{
	Effect::Update();
}

void PlayerAttackEffect::Render(HDC _hdc)
{
	Effect::Render(_hdc);
}