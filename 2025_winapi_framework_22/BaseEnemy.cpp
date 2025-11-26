#include "pch.h"
#include "BaseEnemy.h"
#include "Collider.h"
#include "SceneManager.h"
#include "Rigidbody.h"
BaseEnemy::BaseEnemy()
{
	AddComponent<Collider>();
}
BaseEnemy::~BaseEnemy()
{

}
void BaseEnemy::Update() {
	MoveToTarget(m_deltaTime);       // 목표 지점을 향해 이동
	TryAttack(m_deltaTime);          // 공격 시도 (조건 충족 시 공격)
	m_timeSinceLastAttack += m_deltaTime;  // 공격 쿨타임 경과 시간 누적
}

void BaseEnemy::Render(HDC _hdc)
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

void BaseEnemy::EnterCollision(Collider* _other)
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

void BaseEnemy::StayCollision(Collider* _other)
{
}

void BaseEnemy::ExitCollision(Collider* _other)
{
}

void BaseEnemy::TakeDamage(int damage)
{
}

void BaseEnemy::Death()
{
}

void BaseEnemy::MoveToTarget(float deltaTime)
{
}

void BaseEnemy::TryAttack(float deltaTime)
{
}

bool BaseEnemy::IsInAttackRange() const
{
	return false;
}
