#pragma once
#include "Object.h"
#include "Enums.h"

class Texture;
class Collider;
class Rigidbody;

class Player : public Object
{
public:
	Player();
	~Player();

public:
	void Update() override;
	void Render(HDC _hdc) override;
	void EnterCollision(Collider* _other) override;
	void StayCollision(Collider* _other) override;
	void ExitCollision(Collider* _other) override;
private:
	void UpdateInput();

private:
	Texture* m_pTex;
	float movementSpeed;
	float rollingSpeed;
	bool isRolling;
	Rigidbody* rigidCompo;
};
