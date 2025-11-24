#pragma once
#include "Object.h"
class JewelryBox : public Object
{
public:
	JewelryBox();
	~JewelryBox();
public:
	// Object을(를) 통해 상속됨
	void Update() override;
	void Render(HDC _hdc) override;
	void EnterCollision(Collider* _other) override;
	void StayCollision(Collider* _other) override;
	void ExitCollision(Collider* _other) override;
private:
	int hp;
};

