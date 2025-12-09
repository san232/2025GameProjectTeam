#include "pch.h"
#include "PlayerLevelUpEffect.h"
#include "Animator.h"
#include "Texture.h"
#include "ResourceManager.h"

PlayerLevelUpEffect::PlayerLevelUpEffect()
{
	m_animator = AddComponent<Animator>();

	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerLevelUp");

	m_animator->CreateAnimation(L"Effect", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 64.f, 0.f }, 8, 0.08f);
	m_animator->Play(L"Effect", PlayMode::Once);
}

PlayerLevelUpEffect::~PlayerLevelUpEffect()
{

}

void PlayerLevelUpEffect::Update()
{
	Effect::Update();
}

void PlayerLevelUpEffect::Render(HDC _hdc)
{
	Effect::Render(_hdc);
}