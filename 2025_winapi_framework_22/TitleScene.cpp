#include "pch.h"
#include "TitleScene.h"
#include "Core.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GDISelector.h"
#include "Texture.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{

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
        
        StretchBlt(_hdc, 0, 0, rect.right, rect.bottom, 
                   pTex->GetTextureDC(), 0, 0, texWidth, texHeight, SRCCOPY);
    }

    {
        GDISelector brushSel(_hdc, BrushType::LIGHTGRAY);
        GDISelector penSel(_hdc, PenType::BLACK);

        Rectangle(_hdc, m_btnStart.left, m_btnStart.top, m_btnStart.right, m_btnStart.bottom);
        
        SetTextColor(_hdc, RGB(0, 0, 0));
        DrawText(_hdc, L"START", -1, &m_btnStart, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    {
        GDISelector brushSel(_hdc, BrushType::GRAY);
        GDISelector penSel(_hdc, PenType::BLACK);

        Rectangle(_hdc, m_btnExit.left, m_btnExit.top, m_btnExit.right, m_btnExit.bottom);
        
        SetTextColor(_hdc, RGB(0, 0, 0));
        DrawText(_hdc, L"EXIT", -1, &m_btnExit, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

void TitleScene::Release()
{
    Scene::Release();
}
