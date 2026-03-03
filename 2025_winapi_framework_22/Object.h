#pragma once
#include "Enums.h"
class Component;
class Collider;
class Object
{
public:
	Object();
	virtual ~Object(); //  Ҹ 
public:
	virtual void Update() abstract;
	virtual void LateUpdate();
	void ComponentRender(HDC _hdc);
	virtual void Render(HDC _hdc) abstract;
	virtual void EnterCollision(Collider* _other) {}
	virtual void StayCollision(Collider* _other) {}
	virtual void ExitCollision(Collider* _other) {}
public:
	void SetPos(Vec2 _pos) { m_pos = _pos; }
	void SetSize(Vec2 _size) { m_size = _size; }
	const Vec2& GetPos() const { return m_pos; }
	const Vec2& GetSize() const { return m_size; }
	bool GetIsDead() const { return m_isDie; }
	void SetDead() { m_isDie = true; }
	Layer GetLayer() const { return m_layer; }
	void SetLayer(Layer _layer) { m_layer = _layer; }
protected:
	void Translate(Vec2 _delta)
	{
		m_pos.x += _delta.x;
		m_pos.y += _delta.y;
	}
	void Scale(Vec2 _s)
	{
		m_size.x *= _s.x;
		m_size.y *= _s.y;
	}
public:
	template<typename T>
	T* AddComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "Componentκ ӹ޾ƾ մϴ.");
		T* compo = new T;
		compo->SetOwner(this); //  ã
		// ڱ ڽ ⺻  Ϸ
		// setownerϰ  owner getcom ϴ   
		compo->Init();
		m_vecComponents.push_back(compo);
		return compo; // ϰ ߿ setter  ϰ 
	}
	template<typename T>
	T* GetComponent()
	{
		T* component = nullptr;
		for (Component* com : m_vecComponents)
		{
			component = dynamic_cast<T*>(com);
			if (component)
				break;
		}
		return component;
	}
private:
	bool m_isDie;
	Vec2 m_pos;
	Vec2 m_size;
	Layer m_layer;
	vector<Component*> m_vecComponents;
};

