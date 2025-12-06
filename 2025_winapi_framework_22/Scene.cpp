#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "CollisionManager.h"
#include "Rigidbody.h"
Scene::Scene()
{

}

Scene::~Scene()
{
	Release();
}
void Scene::Release()
{
	FlushEvent();
	for (UINT i = 0; i < (UINT)Layer::DEFAULTEND; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
			SAFE_DELETE(obj);
		vec.clear();
	}
	GET_SINGLE(CollisionManager)->CheckReset();
}

void Scene::RequestDestroy(Object* obj)
{
	if (obj == nullptr)
		return;

	// 죽지 않으면 넣어라
	if (!obj->GetIsDead())
	{
		obj->SetDead();
		// 실제 delete는 FlushObject에서
		m_killObject.push_back(obj);
	}
}
void Scene::FlushEvent()
{
	// 삭제
	for (Object* d : m_killObject)
	{
		RemoveObject(d);
		SAFE_DELETE(d);
	}
	m_killObject.clear();

	for (SpawnObject spawnObj : m_spawnObject)
	{
		AddObject(spawnObj.obj, spawnObj.type);
	}
	m_spawnObject.clear();

}

void Scene::RequestSpawn(Object* obj, Layer _type)
{
	if (obj == nullptr)
		return;

	SpawnObject spawnObj;
	spawnObj.obj = obj;
	spawnObj.type = _type;
	m_spawnObject.push_back(spawnObj);
}


void Scene::RemoveObject(Object* _obj)
{
	for (UINT i = 0; i < (UINT)Layer::DEFAULTEND; ++i)
	{
		auto& v = m_vecObj[i];
		v.erase(std::remove(v.begin(), v.end(), _obj), v.end());
	}

}

void Scene::Update()
{
	//Scene* pScene = new DevScene;
	for (UINT i = 0; i < (UINT)Layer::DEFAULTEND; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
			if(!obj->GetIsDead())
				obj->Update();
	}
}

void Scene::FixedUpdate(float _fixedDT)
{
	for (UINT i = 0; i < (UINT)Layer::DEFAULTEND; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
		{
			if (auto rb = obj->GetComponent<Rigidbody>())
			{
				if (rb != nullptr)
					rb->FixedUpdate(_fixedDT);
			}
		}
	}
}

void Scene::LateUpdate()
{
	for (UINT i = 0; i < (UINT)Layer::DEFAULTEND; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
			if (!obj->GetIsDead())
				obj->LateUpdate();
	}
}
void Scene::Render(HDC _hdc)
{
	for (UINT i = 0; i < (UINT)Layer::DEFAULTEND; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
			if (!obj->GetIsDead())
				obj->Render(_hdc);
	}
}

