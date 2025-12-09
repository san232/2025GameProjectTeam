#pragma once
#include "Effect.h"

class Animator;
class Texture;

class PlayerRollingEffect : public Effect
{
public:
	PlayerRollingEffect();
	~PlayerRollingEffect() override;

public:
	void Update() override;
	void Render(HDC _hdc) override;

private:
	Texture* m_pTex;
	Animator* m_animator;
};