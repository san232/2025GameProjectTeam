#pragma once
#include "Bullet.h"

class HomingBullet : public Bullet
{
public:
	HomingBullet();
	~HomingBullet() override;

public:
	void Render(HDC _hdc) override;
	void SetTarget(Entity* target) { m_target = target; }

protected:
	void Move() override;

private:
	Entity* m_target;
};