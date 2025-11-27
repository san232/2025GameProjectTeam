#include "pch.h"
#include "JewelryBox.h"
#include "Collider.h"

JewelryBox::JewelryBox()
	: m_hp(100)
{
	AddComponent<Collider>();
}

JewelryBox::~JewelryBox()
{

}

void JewelryBox::Update()
{

}

void JewelryBox::Render(HDC _hdc)
{
	Vec2 pos = GetPos();
	Vec2 size = GetSize();

	BrushType brush = BrushType::HOLLOW;
	PenType pen = PenType::GREEN;

	GDISelector brushSelector(_hdc, brush);
	GDISelector penSelector(_hdc, pen);

	RECT_RENDER(_hdc, pos.x, pos.y, size.x, size.y);

	ComponentRender(_hdc);
}

void JewelryBox::EnterCollision(Collider* _other)
{

}

void JewelryBox::StayCollision(Collider* _other)
{

}

void JewelryBox::ExitCollision(Collider* _other)
{

}
