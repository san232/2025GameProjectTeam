#include "pch.h"
#include "SubWindowRenderer.h"
#include "SubWindow.h"
#include "Scene.h"
#include "Object.h"
#include "Core.h"

#pragma comment(lib, "msimg32.lib")

SubWindowRenderer::SubWindowRenderer(HWND inMainWindow, Scene* inScene)
    : mainWindow(inMainWindow)
    , scene(inScene)
    , m_memDC(nullptr)
    , m_hColorBitmap(nullptr)
    , m_lastColor(0xFFFFFFFF)
{
}

SubWindowRenderer::~SubWindowRenderer()
{
    if (m_hColorBitmap)
        ::DeleteObject(m_hColorBitmap);
    if (m_memDC)
        ::DeleteDC(m_memDC);
}

void SubWindowRenderer::Render(HDC subDC, SubWindow* subWin, HDC mainBackDC)
{
    if (!subWin || !subWin->IsActive()) return;
    
    POINT mainPtTL = { 0, 0 };
    ::ClientToScreen(mainWindow, &mainPtTL);
    
    RECT mainClientRect;
    ::GetClientRect(mainWindow, &mainClientRect);
    int mainW = mainClientRect.right - mainClientRect.left;
    int mainH = mainClientRect.bottom - mainClientRect.top;

    RECT mainScreenRect = {
        mainPtTL.x,
        mainPtTL.y,
        mainPtTL.x + mainW,
        mainPtTL.y + mainH
    };

    RECT subClientRect;
    ::GetClientRect(subWin->GetHWnd(), &subClientRect);
    POINT subPtTL = { subClientRect.left, subClientRect.top };
    POINT subPtBR = { subClientRect.right, subClientRect.bottom };
    ::ClientToScreen(subWin->GetHWnd(), &subPtTL);
    ::ClientToScreen(subWin->GetHWnd(), &subPtBR);
    RECT subScreenRect = { subPtTL.x, subPtTL.y, subPtBR.x, subPtBR.y };

    int subW = subClientRect.right - subClientRect.left;
    int subH = subClientRect.bottom - subClientRect.top;

    HDC bufferDC = ::CreateCompatibleDC(subDC);
    HBITMAP bufferBitmap = ::CreateCompatibleBitmap(subDC, subW, subH);
    HBITMAP oldBitmap = (HBITMAP)::SelectObject(bufferDC, bufferBitmap);

    HBRUSH whiteBrush = (HBRUSH)::GetStockObject(WHITE_BRUSH);
    RECT fullRect = { 0, 0, subW, subH };
    ::FillRect(bufferDC, &fullRect, whiteBrush);

    RECT intersect;
    if (::IntersectRect(&intersect, &mainScreenRect, &subScreenRect))
    {
        int srcX = intersect.left - mainScreenRect.left;
        int srcY = intersect.top - mainScreenRect.top;
        
        int width = intersect.right - intersect.left;
        int height = intersect.bottom - intersect.top;

        int dstX = intersect.left - subScreenRect.left;
        int dstY = intersect.top - subScreenRect.top;

        ::BitBlt(bufferDC, dstX, dstY, width, height, mainBackDC, srcX, srcY, SRCCOPY);
    }

    if (!m_memDC) m_memDC = ::CreateCompatibleDC(subDC);

    COLORREF curColor = subWin->GetTintColor();
    if (!m_hColorBitmap || m_lastColor != curColor)
    {
        HBITMAP hNewBmp = ::CreateBitmap(1, 1, 1, 32, NULL);
        HBITMAP oldMemBitmap = (HBITMAP)::SelectObject(m_memDC, hNewBmp);
        
        if (m_hColorBitmap) ::DeleteObject(m_hColorBitmap);
        
        m_hColorBitmap = hNewBmp;
        ::SetPixel(m_memDC, 0, 0, curColor);
        m_lastColor = curColor;
    }

    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = (BYTE)(subWin->GetAlpha() * 255);
    bf.AlphaFormat = 0;

    ::GdiAlphaBlend(bufferDC, 0, 0, subW, subH, m_memDC, 0, 0, 1, 1, bf);

    ::BitBlt(subDC, 0, 0, subW, subH, bufferDC, 0, 0, SRCCOPY);

    ::SelectObject(bufferDC, oldBitmap);
    ::DeleteObject(bufferBitmap);
    ::DeleteDC(bufferDC);
}

void SubWindowRenderer::RenderLegacy(HDC hdc)
{
    if (scene)
    {
        scene->Render(hdc);
    }
}