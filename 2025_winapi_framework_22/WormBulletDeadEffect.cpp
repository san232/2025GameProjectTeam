#include "pch.h"
#include "Animator.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "WormBulletDeadEffect.h"

WormBulletDeadEffect::WormBulletDeadEffect()
{
	m_animator = AddComponent<Animator>();

	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"WormBulletDeadEffect");

	m_animator->CreateAnimation(L"Effect", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 64.f, 0.f }, 7, 0.05f);
	m_animator->Play(L"Effect", PlayMode::Once);
	m_animator->SetScaleRatio(2.f);
}

WormBulletDeadEffect::~WormBulletDeadEffect()
{

}

void WormBulletDeadEffect::Update()
{
	Effect::Update();
}

void WormBulletDeadEffect::Render(HDC _hdc)
{
	Effect::Render(_hdc);
}