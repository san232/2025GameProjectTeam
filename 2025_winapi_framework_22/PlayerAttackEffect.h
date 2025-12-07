#pragma once
#include "Effect.h"

class Animator;

class PlayerAttackEffect : public Effect
{
public:
	PlayerAttackEffect();
	~PlayerAttackEffect() override;

public:
	void Update() override;
	void Render(HDC _hdc) override;

private:
	Animator* m_animator;
};