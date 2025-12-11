#include "pch.h"
#include "DeadScene.h"
#include "Texture.h"
#include "Core.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "EnemySpawnManager.h"
#include "ResourceManager.h"
#include "GDISelector.h"

DeadScene::DeadScene()
{
}

DeadScene::~DeadScene()
{
}

void DeadScene::Init()
{
    GET_SINGLE(ResourceManager)->Play(L"GameOver");
    RECT rect;
    GetClientRect(GET_SINGLE(Core)->GetHwnd(), &rect);

    int btnWidth = 200;
    int btnHeight = 60;
    
    int centerX = (rect.right - rect.left) / 2;
    int centerY = (rect.bottom - rect.top) / 2;

    m_btnRetry.left = centerX - (btnWidth / 2);
    m_btnRetry.top = centerY + 50; 
    m_btnRetry.right = centerX + (btnWidth / 2);
    m_btnRetry.bottom = centerY + 50 + btnHeight;

    m_btnExit.left = centerX - (btnWidth / 2);
    m_btnExit.top = centerY + 120;
    m_btnExit.right = centerX + (btnWidth / 2);
    m_btnExit.bottom = centerY + 120 + btnHeight;
}

void DeadScene::Update()
{
    Scene::Update();

    if (GET_KEYDOWN(KEY_TYPE::LBUTTON))
    {
        POINT mousePos = GET_MOUSEPOS;

        if (PtInRect(&m_btnRetry, mousePos))
        {
            GET_SINGLE(EnemySpawnManager)->ResetWave();
            GET_SINGLE(SceneManager)->LoadSceneWithTransition(L"LSScene");
        }

        if (PtInRect(&m_btnExit, mousePos))
        {
            PostQuitMessage(0);
        }
    }
}

void DeadScene::Render(HDC _hdc)
{
    Scene::Render(_hdc);

    RECT rect;
    ::GetClientRect(GET_SINGLE(Core)->GetHwnd(), &rect);

    Texture* pTex = GET_SINGLE(ResourceManager)->GetTexture(L"GameOverScene");
    if (pTex != nullptr)
    {
        int texWidth = pTex->GetWidth();
        int texHeight = pTex->GetHeight();

        StretchBlt(_hdc, 0, 0, rect.right, rect.bottom,
            pTex->GetTextureDC(), 0, 0, texWidth, texHeight, SRCCOPY);
    }

    int oldBkMode = SetBkMode(_hdc, TRANSPARENT);

    {
        GDISelector brushSel(_hdc, BrushType::LIGHTGRAY);
        GDISelector penSel(_hdc, PenType::BLACK);

        Rectangle(_hdc, m_btnRetry.left, m_btnRetry.top, m_btnRetry.right, m_btnRetry.bottom);
        
        SetTextColor(_hdc, RGB(0, 0, 0));
        DrawText(_hdc, L"RETRY", -1, &m_btnRetry, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    {
        GDISelector brushSel(_hdc, BrushType::GRAY);
        GDISelector penSel(_hdc, PenType::BLACK);

        Rectangle(_hdc, m_btnExit.left, m_btnExit.top, m_btnExit.right, m_btnExit.bottom);

        SetTextColor(_hdc, RGB(0, 0, 0));
        DrawText(_hdc, L"EXIT", -1, &m_btnExit, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    SetBkMode(_hdc, oldBkMode);
}

void DeadScene::Release()
{
    GET_SINGLE(ResourceManager)->Stop(SOUND_CHANNEL::BGM);
    Scene::Release();
}
