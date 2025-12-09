#pragma once
#include "Effect.h"

class Animator;
class Texture;

class PlayerLevelUpEffect : public Effect
{
public:
	PlayerLevelUpEffect();
	~PlayerLevelUpEffect() override;

public:
	void Update() override;
	void Render(HDC _hdc) override;

private:
	Texture* m_pTex;
	Animator* m_animator;
};
