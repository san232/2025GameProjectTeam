#include "pch.h"
#include "EnemyHitEffect.h"
#include "Animator.h"
#include "Texture.h"
#include "ResourceManager.h"

EnemyHitEffect::EnemyHitEffect()
{
	m_animator = AddComponent<Animator>();

	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"EnemyHitEffect");

	m_animator->CreateAnimation(L"Effect", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 64.f, 0.f }, 6, 0.05f);
	m_animator->Play(L"Effect", PlayMode::Once);
}

EnemyHitEffect::~EnemyHitEffect()
{

}

void EnemyHitEffect::Update()
{
	Effect::Update();
}

void EnemyHitEffect::Render(HDC _hdc)
{
	Effect::Render(_hdc);
}