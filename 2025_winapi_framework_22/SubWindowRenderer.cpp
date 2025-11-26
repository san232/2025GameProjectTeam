#include "pch.h"
#include "SubWindowRenderer.h"
#include "Scene.h"
#include "Object.h"
#include "Core.h"

SubWindowRenderer::SubWindowRenderer(HWND inMainWindow, Scene* inScene)
    : mainWindow(inMainWindow)
    , scene(inScene)
{
}

void SubWindowRenderer::Render(HDC hdc, const POINT& clientPos)
{
    const int width = 160;
    const int height = 160;

    RECT parentRc = {};
    ::GetClientRect(mainWindow, &parentRc);
    if (parentRc.right <= 0 || parentRc.bottom <= 0)
        return;

    int sx = clientPos.x - (width / 2);
    int sy = clientPos.y - (height / 2);
    if (sx < 0) sx = 0;
    if (sy < 0) sy = 0;
    if (sx + width > parentRc.right)  sx = (parentRc.right - width > 0) ? parentRc.right - width : 0;
    if (sy + height > parentRc.bottom) sy = (parentRc.bottom - height > 0) ? parentRc.bottom - height : 0;

    HDC backDC = GET_SINGLE(Core)->GetBackDC();
    if (backDC != nullptr)
    {
        ::BitBlt(hdc, 0, 0, width, height, backDC, sx, sy, SRCCOPY);
    }

    int saved = ::SaveDC(hdc);

    HRGN clip = ::CreateRectRgn(0, 0, width, height);
    ::SelectClipRgn(hdc, clip);
    ::DeleteObject(clip);

    ::SetBkMode(hdc, TRANSPARENT);
    ::SetViewportOrgEx(hdc, -sx, -sy, nullptr);

    const std::vector<Object*>& ref = scene->GetLayerObjects(Layer::INVISIBLEENEMY);
    std::vector<Object*> snapshot(ref.begin(), ref.end());

    for (Object* obj : snapshot)
    {
        if (obj != nullptr)
            obj->Render(hdc);
    }

    ::RestoreDC(hdc, saved);
}
