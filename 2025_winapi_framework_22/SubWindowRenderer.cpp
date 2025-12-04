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

void SubWindowRenderer::Render(HDC subDC, SubWindow* subWin, HDC mainBackDC, RECT mainWndRect)
{
    if (!subWin || !subWin->IsActive()) return;
    
    RECT subRect = subWin->GetRect();
    int subW = subRect.right - subRect.left;
    int subH = subRect.bottom - subRect.top;

    HBRUSH whiteBrush = (HBRUSH)::GetStockObject(WHITE_BRUSH);
    RECT fullRect = { 0, 0, subW, subH };
    ::FillRect(subDC, &fullRect, whiteBrush);

    RECT intersect;
    if (::IntersectRect(&intersect, &mainWndRect, &subRect))
    {
        POINT ptTL = { intersect.left, intersect.top };
        ::ScreenToClient(mainWindow, &ptTL);
        
        int srcX = ptTL.x;
        int srcY = ptTL.y;
        int width = intersect.right - intersect.left;
        int height = intersect.bottom - intersect.top;

        POINT ptDst = { intersect.left, intersect.top };
        ::ScreenToClient(subWin->GetHWnd(), &ptDst);
        
        int dstX = ptDst.x;
        int dstY = ptDst.y;

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
