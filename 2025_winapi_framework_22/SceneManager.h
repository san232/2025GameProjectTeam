#pragma once
#include "Scene.h"
class Object;

class SceneManager
{
	DECLARE_SINGLE(SceneManager);
public:
	void Init();
	void Update();
	void FixedUpdate(float _fixedDT);
	void PhysicsSyncColliders();
	void Render(HDC _hdc);
	
public:
	void RegisterScene(const wstring& _name, std::shared_ptr<Scene> _scene);
	void LoadScene(const wstring& _name);
	void LoadSceneWithTransition(const wstring& _targetName);
	void RequestDestroy(Object* _obj);
public:
	const std::shared_ptr<Scene>& GetCurScene() const
	{
		return m_curScene;
	}

private:
	void RenderTransition(HDC _hdc);

private:
	std::shared_ptr<Scene> m_curScene;
	std::unordered_map<wstring, std::shared_ptr<Scene>> m_mapScene;

	TransitionMode m_transitionMode;
	wstring        m_nextSceneName;
	float          m_transitionTimer;
	float          m_transitionDuration;
};