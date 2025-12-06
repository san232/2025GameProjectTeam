#include "pch.h"
#include "Effect.h"
#include "TimeManager.h"

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
	if (m_duration > 0.f)
	{
		m_accTime += fDT;
		if (m_accTime >= m_duration)
		{
			SetDead();
		}
	}
}

void Effect::Render(HDC _hdc)
{
	ComponentRender(_hdc);
}
