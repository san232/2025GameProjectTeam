#include "pch.h"
#include "SubWindowController.h"
#include "InputManager.h"

SubWindowController::SubWindowController(HWND subWindow, SIZE windowSize)
    : subWindow(subWindow), size(windowSize), m_isMoving(false)
{
}

void SubWindowController::Update()
{
    if (!m_isMoving)
        return;

    if (subWindow == nullptr)
        return;

    HWND parent = ::GetParent(subWindow);
    if (parent == nullptr)
        return;

    POINT cursor = GET_MOUSEPOS;

    RECT parentRect = {};
    ::GetClientRect(parent, &parentRect);

    int targetX = cursor.x - (size.cx / 2);
    int targetY = cursor.y - (size.cy / 2);

    const int maxX = max(0L, parentRect.right - (LONG)(size.cx));
    const int maxY = max(0L, parentRect.bottom - (LONG)(size.cy));
    targetX = min(max(0, targetX), maxX);
    targetY = min(max(0, targetY), maxY);

    RECT wndRect = {};
    ::GetWindowRect(subWindow, &wndRect);

    POINT currentTL = { wndRect.left, wndRect.top };
    ::ScreenToClient(parent, &currentTL);

    if (currentTL.x == targetX && currentTL.y == targetY)
        return;

    ::SetWindowPos(
        subWindow,
        nullptr,
        targetX, targetY,
        0, 0,
        SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER
    );
}