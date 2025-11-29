#pragma once
#include "Object.h"

class Entity : public Object
{
public:
	Entity();
	~Entity();
public:
	// Object을(를) 통해 상속됨
	void Update() override;
	void Render(HDC _hdc) override;
protected:
	virtual void Attack() abstract;
	virtual void Dead() abstract;
	virtual void Move() abstract;
};

