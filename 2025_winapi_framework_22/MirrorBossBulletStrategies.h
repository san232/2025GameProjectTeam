#pragma once
#include "IBulletStrategy.h"
#include "FastBullet.h"
#include "HomingBullet.h"
#include "BoomerangBullet.h"
#include "FragmentingBullet.h"
#include "SceneManager.h"
#include "Scene.h"

class FastBulletStrategy : public IBulletStrategy
{
public:
    void Fire(MirrorBoss* _boss, Entity* _target, const Vec2& _pos, const Vec2& _dir, int _damage) override
    {
        FastBullet* bullet = new FastBullet();
        bullet->SetPos(_pos);
        bullet->SetDirection(_dir);
        bullet->SetAttackPower(_damage);
        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(bullet, Layer::ENEMYBULLET);
    }
};

class HomingBulletStrategy : public IBulletStrategy
{
public:
    void Fire(MirrorBoss* _boss, Entity* _target, const Vec2& _pos, const Vec2& _dir, int _damage) override
    {
        HomingBullet* bullet = new HomingBullet();
        bullet->SetPos(_pos);
        bullet->SetDirection(_dir);
        bullet->SetTarget(_target);
        bullet->SetAttackPower((int)(_damage * 0.8f));
        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(bullet, Layer::ENEMYBULLET);
    }
};

class BoomerangBulletStrategy : public IBulletStrategy
{
public:
    void Fire(MirrorBoss* _boss, Entity* _target, const Vec2& _pos, const Vec2& _dir, int _damage) override
    {
        BoomerangBullet* bullet = new BoomerangBullet();
        bullet->SetPos(_pos);
        bullet->SetDirection(_dir);
        bullet->SetAttackPower((int)(_damage * 1.2f));
        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(bullet, Layer::ENEMYBULLET);
    }
};

class FragmentingBulletStrategy : public IBulletStrategy
{
public:
    void Fire(MirrorBoss* _boss, Entity* _target, const Vec2& _pos, const Vec2& _dir, int _damage) override
    {
        FragmentingBullet* bullet = new FragmentingBullet();
        bullet->SetPos(_pos);
        bullet->SetDirection(_dir);
        bullet->SetAttackPower((int)(_damage * 0.5f));
        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(bullet, Layer::ENEMYBULLET);
    }
};
