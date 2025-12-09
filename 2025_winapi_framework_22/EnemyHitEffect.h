#pragma once
#include "Effect.h"

class Animator;
class Texture;

class EnemyHitEffect : public Effect
{
public:
	EnemyHitEffect();
	~EnemyHitEffect() override;

public:
	void Update() override;
	void Render(HDC _hdc) override;

private:
	Texture* m_pTex;
	Animator* m_animator;
};