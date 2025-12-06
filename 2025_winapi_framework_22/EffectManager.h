#pragma once
class Effect;

class EffectManager
{
	DECLARE_SINGLE(EffectManager);

public:
	void Init();

	template <typename T>
	T* CreateEffect(Vec2 _pos, Vec2 _size, float _duration = -1.f)
	{
		static_assert(std::is_base_of<Effect, T>::value, "Effect 클래스를 상속받아야 합니다.");

		T* pEffect = new T;
		pEffect->SetPos(_pos);
		pEffect->SetSize(_size);

		if (_duration > 0.f)
			pEffect->SetDuration(_duration);

		if (!RegisterEffect(pEffect))
		{
			delete pEffect;
			return nullptr;
		}

		return pEffect;
	}

private:
	bool RegisterEffect(Effect* _effect);
};