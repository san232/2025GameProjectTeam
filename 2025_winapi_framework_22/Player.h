#pragma once
#include "Object.h"
class Texture;
class Player :
    public Object
{
public:
	Player();
	~Player();
public:
	//virtual void Update();
	//virtual void Render(HDC _hdc);
	void Update() override;
	void Render(HDC _hdc) override;
	void EnterCollision(Collider* _other)override;
	void StayCollision(Collider* _other) override;
	void ExitCollision(Collider* _other) override;
private:
	Texture* m_pTex;
};

