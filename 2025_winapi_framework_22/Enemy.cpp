#include "pch.h"
#include "Enemy.h"
#include "Collider.h"
#include "SceneManager.h"
#include "Rigidbody.h"
Enemy::Enemy()
{
	AddComponent<Collider>();
}
Enemy::~Enemy()
{
}
void Enemy::Update()
{
}

void Enemy::Render(HDC _hdc)
{
	//HBRUSH hbrush = ::CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
	//HBRUSH holdbrush = (HBRUSH)::SelectObject(_hdc, hbrush);
	
	Vec2 pos = GetPos();
	Vec2 size = GetSize();
	RECT_RENDER(_hdc, pos.x, pos.y
		, size.x, size.y);

	//::SelectObject(_hdc, holdbrush);
	//::DeleteObject(hbrush);
	ComponentRender(_hdc);
}

void Enemy::EnterCollision(Collider* _other)
{
	cout << "Enter" << endl;
	if (_other->IsTrigger())
	{
		if (_other->GetName() == L"")
		{
			// 뭔지 확인하고 삭제.
			//GET_SINGLE(SceneManager)->RequestDestroy(this);
			//GET_SINGLE(SceneManager)->RequestDestroy(_other->GetOwner());
		}
	}
	// 물리충돌
	else
	{


	}
}

void Enemy::StayCollision(Collider* _other)
{
	cout << "Stay" << endl;
}

void Enemy::ExitCollision(Collider* _other)
{
	cout << "Exit" << endl;
}
