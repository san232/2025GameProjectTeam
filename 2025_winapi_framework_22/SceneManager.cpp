#include "pch.h"
#include "SceneManager.h"
#include "LCMScene.h"
#include "LSScene.h"
#include "TitleScene.h"
#include "Collider.h"
#include "Object.h"
void SceneManager::Init()
{
	m_curScene = nullptr;

	// todo
	//dynamic_cast<> 
	//std::dynamic_pointer_cast<>

	// �� ���
	RegisterScene(L"TitleScene", std::make_shared<TitleScene>());
	RegisterScene(L"LCMScene", std::make_shared<LCMScene>());
	RegisterScene(L"LSScene", std::make_shared<LSScene>());

	// �� �ε�
	LoadScene(L"TitleScene");
	//LoadScene(L"LCMScene");
	//LoadScene(L"LSScene");
}

void SceneManager::RegisterScene(const wstring& _name, std::shared_ptr<Scene> _scene)
{
	if (_name.empty() || _scene == nullptr)
		return;
	m_mapScene.insert(m_mapScene.end(), { _name, _scene });
}

void SceneManager::Update()
{
	if (m_curScene == nullptr)
		return;
	m_curScene->Update();
	m_curScene->LateUpdate();
}

void SceneManager::FixedUpdate(float _fixedDT)
{
	// �������
	if (m_curScene == nullptr)
		return;
	m_curScene->FixedUpdate(_fixedDT);
	PhysicsSyncColliders();
}
void SceneManager::PhysicsSyncColliders()
{
	for (UINT i = 0; i < (UINT)Layer::DEFAULTEND; ++i)
	{
		const auto& objects = m_curScene->GetLayerObjects((Layer)i);
		for (Object* obj : objects)
		{
			if (!obj)
				continue;

			if (auto* col = obj->GetComponent<Collider>())
				col->LateUpdate(); // sync  
		}
	}
}
void SceneManager::Render(HDC _hdc)
{
	if (m_curScene == nullptr)
		return;
	m_curScene->Render(_hdc);
}


void SceneManager::LoadScene(const wstring& _name)
{
	// change
	if (m_curScene != nullptr)
	{
		m_curScene->Release();
		m_curScene = nullptr;
	}

	auto iter = m_mapScene.find(_name);
	if (iter != m_mapScene.end())
	{
		m_curScene = iter->second;
		m_curScene->Init();
	}
}
