#pragma once
#include "Effect.h"

class Animator;
class Texture;

class CatBulletDeadEffect : public Effect
{
public:
	CatBulletDeadEffect();
	~CatBulletDeadEffect() override;

public:
	void Update() override;
	void Render(HDC _hdc) override;

private:
	Texture* m_pTex;
	Animator* m_animator;
};