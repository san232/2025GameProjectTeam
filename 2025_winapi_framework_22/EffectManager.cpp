#include "pch.h"
#include "EffectManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Effect.h"
#include "Object.h"


void EffectManager::Init()
{
}

bool EffectManager::RegisterEffect(Effect* _effect)
{
	Scene* curScene = SceneManager::GetInst()->GetCurScene().get();
	if (curScene)
	{
		curScene->RequestSpawn(_effect, Layer::VFX);
		return true;
	}
	
	return false;
}
//template <typename T>
//T* EffectManager::CreateEffect(Vec2 _pos, Vec2 _size, float _duration)
//{
//	static_assert(std::is_base_of<Effect, T>::value, "Effect Ŭ������ ��ӹ޾ƾ� �մϴ�.");
//
//	T* pEffect = new T;
//	pEffect->SetPos(_pos);
//	pEffect->SetSize(_size);
//
//	if (_duration > 0.f)
//		pEffect->SetDuration(_duration);
//
//	Scene* curScene = SceneManager::GetInst()->GetCurScene().get();
//	if (curScene)
//	{
//		curScene->RequestSpawn(pEffect, Layer::VFX);
//	}
//	else
//	{
//		delete pEffect;
//		return nullptr;
//	}
//
//	return pEffect;
//}