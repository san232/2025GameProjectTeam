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
{
}

SubWindowRenderer::~SubWindowRenderer()
{
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

    HBRUSH whiteBrush = (HBRUSH)::GetStockObject(WHITE_BRUSH);
    RECT fullRect = { 0, 0, subW, subH };
    ::FillRect(subDC, &fullRect, whiteBrush);

    RECT intersect;
    if (::IntersectRect(&intersect, &mainScreenRect, &subScreenRect))
    {
        int srcX = intersect.left - mainScreenRect.left;
        int srcY = intersect.top - mainScreenRect.top;
        
        int width = intersect.right - intersect.left;
        int height = intersect.bottom - intersect.top;

        int dstX = intersect.left - subScreenRect.left;
        int dstY = intersect.top - subScreenRect.top;

        ::BitBlt(subDC, dstX, dstY, width, height, mainBackDC, srcX, srcY, SRCCOPY);
    }

    static HDC memDC = nullptr;
    static HBITMAP hColorBitmap = nullptr;
    static COLORREF lastColor = 0xFFFFFFFF;

    if (!memDC) memDC = ::CreateCompatibleDC(subDC);

    COLORREF curColor = subWin->GetTintColor();
    if (!hColorBitmap || lastColor != curColor)
    {
        if (hColorBitmap) ::DeleteObject(hColorBitmap);
        hColorBitmap = ::CreateBitmap(1, 1, 1, 32, NULL);
        ::SelectObject(memDC, hColorBitmap);
        ::SetPixel(memDC, 0, 0, curColor);
        lastColor = curColor;
    }

    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = (BYTE)(subWin->GetAlpha() * 255);
    bf.AlphaFormat = 0;

    ::GdiAlphaBlend(subDC, 0, 0, subW, subH, memDC, 0, 0, 1, 1, bf);
}

void SubWindowRenderer::RenderLegacy(HDC hdc)
{
    if (scene)
    {
        scene->Render(hdc);
    }
}