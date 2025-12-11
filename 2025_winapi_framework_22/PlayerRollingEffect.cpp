#include "pch.h"
#include "PlayerRollingEffect.h"
#include "Animator.h"
#include "Texture.h"
#include "ResourceManager.h"

PlayerRollingEffect::PlayerRollingEffect()
{
	m_animator = AddComponent<Animator>();

	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerRollingEffect");

	m_animator->CreateAnimation(L"Effect", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 64.f, 0.f }, 10, 0.05f);
	m_animator->Play(L"Effect", PlayMode::Once);
	m_animator->SetScaleRatio(2.f);
}

PlayerRollingEffect::~PlayerRollingEffect()
{
}

void PlayerRollingEffect::Update()
{
	Effect::Update();
}

void PlayerRollingEffect::Render(HDC _hdc)
{
	Effect::Render(_hdc);
}