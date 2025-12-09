#include "pch.h"
#include "SceneManager.h"
#include "LCMScene.h"
#include "LSScene.h"
#include "TitleScene.h"
#include "DeadScene.h"
#include "Collider.h"
#include "Object.h"
#include "Scene.h"
#include "Core.h"
#include "GDISelector.h"
#include "TimeManager.h"

void SceneManager::Init()
{
	m_curScene = nullptr;
    m_transitionMode = TransitionMode::None;
    m_transitionTimer = 0.f;
    m_transitionDuration = 1.0f;

	RegisterScene(L"TitleScene", std::make_shared<TitleScene>());
	RegisterScene(L"DeadScene", std::make_shared<DeadScene>());
	RegisterScene(L"LCMScene", std::make_shared<LCMScene>());
	RegisterScene(L"LSScene", std::make_shared<LSScene>());

	LoadScene(L"TitleScene");
}

void SceneManager::RegisterScene(const wstring& _name, std::shared_ptr<Scene> _scene)
{
	if (_name.empty() || _scene == nullptr)
		return;
	m_mapScene.insert(m_mapScene.end(), { _name, _scene });
}

void SceneManager::Update()
{
	if (m_curScene)
	{
		m_curScene->Update();
		m_curScene->LateUpdate();
	}

    if (m_transitionMode != TransitionMode::None)
    {
        if (m_transitionMode == TransitionMode::FadeOut)
        {
            m_transitionTimer += fDT;
            if (m_transitionTimer >= m_transitionDuration)
            {
                LoadScene(m_nextSceneName);
                m_transitionMode = TransitionMode::FadeIn;
            }
        }
        else if (m_transitionMode == TransitionMode::FadeIn)
        {
            m_transitionTimer -= fDT;
            if (m_transitionTimer <= 0.f)
            {
                m_transitionTimer = 0.f;
                m_transitionMode = TransitionMode::None;
            }
        }
    }
}

void SceneManager::FixedUpdate(float _fixedDT)
{
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
				col->LateUpdate(); 
		}
	}
}

void SceneManager::Render(HDC _hdc)
{
	if (m_curScene)
    {
		m_curScene->Render(_hdc);
    }

    if (m_transitionMode != TransitionMode::None)
    {
        RenderTransition(_hdc);
    }
}


void SceneManager::LoadScene(const wstring& _name)
{
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

void SceneManager::LoadSceneWithTransition(const wstring& _targetName)
{
    if (m_transitionMode != TransitionMode::None) return;

    m_nextSceneName = _targetName;
    m_transitionMode = TransitionMode::FadeOut;
    m_transitionTimer = 0.f;
}

void SceneManager::RequestDestroy(Object* _obj)
{
    if (m_curScene)
        m_curScene->RequestDestroy(_obj);
}

void SceneManager::RenderTransition(HDC _hdc)
{
    RECT rect;
    GetClientRect(GET_SINGLE(Core)->GetHwnd(), &rect);
    
    int centerX = (rect.right - rect.left) / 2;
    int centerY = (rect.bottom - rect.top) / 2;
     
    float maxRadius = sqrt(pow(rect.right, 2) + pow(rect.bottom, 2));
    
    float ratio = m_transitionTimer / m_transitionDuration;
    if (ratio > 1.f) ratio = 1.f;
    if (ratio < 0.f) ratio = 0.f;
    
    float currentRadius = maxRadius * ratio;

    GDISelector brushSel(_hdc, BrushType::BLACK);
    HPEN hNullPen = (HPEN)GetStockObject(NULL_PEN);
    HPEN hOldPen = (HPEN)SelectObject(_hdc, hNullPen);

    Ellipse(_hdc, 
        centerX - (int)currentRadius, 
        centerY - (int)currentRadius, 
        centerX + (int)currentRadius, 
        centerY + (int)currentRadius);

    SelectObject(_hdc, hOldPen);
}