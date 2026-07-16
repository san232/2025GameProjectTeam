#include "pch.h"
#include "Collider.h"
#include "Object.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
UINT Collider::m_nextID = 0;
Collider::Collider()
	: m_size{30.f,30.f}
	, m_updatedPos{0.f,0.f}
	, m_offsetPos{0.f,0.f}
	, m_ID(m_nextID++)
	, m_showDebug(false)
{

}
void Collider::LateUpdate()
{
	// ������Ʈ ���󰡰�
	Object* owner = GetOwner();
	Vec2 pos = owner->GetPos();
	m_updatedPos = pos + m_offsetPos;
}

void Collider::Render(HDC _hDC)
{
	XMFLOAT4 color = m_showDebug ? XMFLOAT4(1, 0, 0, 1) : XMFLOAT4(0, 1, 0, 1);
	float dx = m_updatedPos.x - m_size.x / 2.0f;
	float dy = m_updatedPos.y - m_size.y / 2.0f;
	
	// Un-comment to show debug colliders
	// GET_SINGLE(SpriteRenderer)->DrawRect(dx, dy, m_size.x, m_size.y, color);
}
void Collider::EnterCollision(Collider* _other)
{
	m_showDebug = true;
	GetOwner()->EnterCollision(_other);
}
void Collider::StayCollision(Collider* _other)
{
	GetOwner()->StayCollision(_other);

}
void Collider::ExitCollision(Collider* _other)
{
	m_showDebug = false;
	GetOwner()->ExitCollision(_other);

}
Collider::~Collider()
{

}

void Collider::Init()
{

}


