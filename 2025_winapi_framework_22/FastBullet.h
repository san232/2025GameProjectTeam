#pragma once
#include "Bullet.h"

class FastBullet : public Bullet
{
public:
	FastBullet();
	~FastBullet() override;

public:
	void Render(HDC _hdc) override;

protected:
	void Move() override;
};
