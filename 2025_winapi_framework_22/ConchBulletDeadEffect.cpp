#include "pch.h"
#include "ConchBulletDeadEffect.h"
#include "Animator.h"
#include "Texture.h"
#include "ResourceManager.h"

ConchBulletDeadEffect::ConchBulletDeadEffect()
{
	m_animator = AddComponent<Animator>();

	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"ConchBulletDeadEffect");

	m_animator->CreateAnimation(L"Effect", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 64.f, 0.f }, 7, 0.05f);
	m_animator->Play(L"Effect", PlayMode::Once);
}

ConchBulletDeadEffect::~ConchBulletDeadEffect()
{

}

void ConchBulletDeadEffect::Update()
{
	Effect::Update();
}

void ConchBulletDeadEffect::Render(HDC _hdc)
{
	Effect::Render(_hdc);
}