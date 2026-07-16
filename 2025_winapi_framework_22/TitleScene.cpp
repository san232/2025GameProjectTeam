#include "pch.h"
#include "SpriteRenderer.h"
#include "TitleScene.h"
#include "Core.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GDISelector.h"
#include "Texture.h"
#include "Zombie.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
    GET_SINGLE(ResourceManager)->Play(L"Title");
    RECT rect;
    GetClientRect(GET_SINGLE(Core)->GetHwnd(), &rect);

    int btnWidth = 150;
    int btnHeight = 50;
    
    int centerX = (rect.right - rect.left) / 2;
    int startBtnY = rect.bottom - 150;

    m_btnStart.left = centerX - (btnWidth / 2);
    m_btnStart.top = startBtnY;
    m_btnStart.right = centerX + (btnWidth / 2);
    m_btnStart.bottom = startBtnY + btnHeight;

    int margin = 20;
    m_btnExit.left = rect.right - 100 - margin;
    m_btnExit.top = rect.bottom - 40 - margin;
    m_btnExit.right = rect.right - margin;
    m_btnExit.bottom = rect.bottom - margin;

    Spawn<Zombie>(Layer::DEFAULTENEMY, { 10000.f, 10000.f }, { 100.f, 100.f });
}

void TitleScene::Update()
{
    Scene::Update();

    if (GET_KEYDOWN(KEY_TYPE::LBUTTON))
    {
        POINT mousePos = GET_MOUSEPOS;

        if (PtInRect(&m_btnStart, mousePos))
        {
            GET_SINGLE(SceneManager)->LoadSceneWithTransition(L"LSScene");
        }

        if (PtInRect(&m_btnExit, mousePos))
        {
            PostQuitMessage(0);
        }
    }
}

void TitleScene::Render(HDC _hdc)
{
    Scene::Render(_hdc);

    RECT rect;
    ::GetClientRect(GET_SINGLE(Core)->GetHwnd(), &rect);

    Texture* pTex = GET_SINGLE(ResourceManager)->GetTexture(L"Title");
    if (pTex != nullptr)
    {
        int texWidth = pTex->GetWidth();
        int texHeight = pTex->GetHeight();
        
        GET_SINGLE(SpriteRenderer)->Draw(pTex->GetSRV(), 0, 0, (float)rect.right, (float)rect.bottom, 0, 0, (float)texWidth, (float)texHeight, (float)texWidth, (float)texHeight);
    }

    {
        GET_SINGLE(SpriteRenderer)->DrawFilledRect((float)m_btnStart.left, (float)m_btnStart.top, (float)(m_btnStart.right - m_btnStart.left), (float)(m_btnStart.bottom - m_btnStart.top), XMFLOAT4(0.827f, 0.827f, 0.827f, 1.0f));
        GET_SINGLE(SpriteRenderer)->DrawRect((float)m_btnStart.left, (float)m_btnStart.top, (float)(m_btnStart.right - m_btnStart.left), (float)(m_btnStart.bottom - m_btnStart.top), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
//         DrawText(_hdc, L"START", -1, &m_btnStart, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    {
        GET_SINGLE(SpriteRenderer)->DrawFilledRect((float)m_btnExit.left, (float)m_btnExit.top, (float)(m_btnExit.right - m_btnExit.left), (float)(m_btnExit.bottom - m_btnExit.top), XMFLOAT4(0.502f, 0.502f, 0.502f, 1.0f));
        GET_SINGLE(SpriteRenderer)->DrawRect((float)m_btnExit.left, (float)m_btnExit.top, (float)(m_btnExit.right - m_btnExit.left), (float)(m_btnExit.bottom - m_btnExit.top), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
//         DrawText(_hdc, L"EXIT", -1, &m_btnExit, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

void TitleScene::Release()
{
    GET_SINGLE(ResourceManager)->Stop(SOUND_CHANNEL::BGM);
    Scene::Release();
}
