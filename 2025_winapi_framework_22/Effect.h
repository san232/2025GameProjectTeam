#pragma once
#include "Object.h"

class Effect : public Object
{
public:
	Effect();
	virtual ~Effect();

public:
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hdc) override;

public:
	void SetDuration(float _duration) { m_duration = _duration; }

private:
	float m_duration;
	float m_accTime;
};
