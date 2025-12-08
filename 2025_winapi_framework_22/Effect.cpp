#include "pch.h"
#include "Effect.h"
#include "TimeManager.h"
#include "SceneManager.h"

Effect::Effect()
	: m_duration(-1.f)
	, m_accTime(0.f)
{
}

Effect::~Effect()
{
}

void Effect::Update()
{
}

void Effect::LateUpdate()
{
	Object::LateUpdate();

	if (m_duration > 0.f)
	{
		m_accTime += fDT;
		if (m_accTime >= m_duration)
		{
			SceneManager::GetInst()->RequestDestroy(this);
		}
	}
}

void Effect::Render(HDC _hdc)
{
	ComponentRender(_hdc);
}