#include "pch.h"
#include "PlayerHitEffect.h"
#include "Animator.h"
#include "Texture.h"
#include "ResourceManager.h"

PlayerHitEffect::PlayerHitEffect()
{
	m_animator = AddComponent<Animator>();

	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerHitEffect");

	m_animator->CreateAnimation(L"Effect", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 64.f, 0.f }, 6, 0.05f);
	m_animator->Play(L"Effect", PlayMode::Once);
}

PlayerHitEffect::~PlayerHitEffect()
{

}

void PlayerHitEffect::Update()
{
	Effect::Update();
}

void PlayerHitEffect::Render(HDC _hdc)
{
	Effect::Render(_hdc);
}