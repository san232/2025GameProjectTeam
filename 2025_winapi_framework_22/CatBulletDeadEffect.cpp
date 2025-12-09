#include "pch.h"
#include "CatBulletDeadEffect.h"
#include "Animator.h"
#include "Texture.h"
#include "ResourceManager.h"

CatBulletDeadEffect::CatBulletDeadEffect()
{
	m_animator = AddComponent<Animator>();

	m_pTex = GET_SINGLE(ResourceManager)->GetTexture(L"CatBulletDeadEffect");

	m_animator->CreateAnimation(L"Effect", m_pTex, { 0.f, 0.f }, { 64.f, 64.f }, { 64.f, 0.f }, 5, 0.05f);
	m_animator->Play(L"Effect", PlayMode::Once);
}

CatBulletDeadEffect::~CatBulletDeadEffect()
{

}

void CatBulletDeadEffect::Update()
{
	Effect::Update();
}

void CatBulletDeadEffect::Render(HDC _hdc)
{
	Effect::Render(_hdc);
}