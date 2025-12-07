#include "pch.h"
#include "SubWindowController.h"
#include "InputManager.h"
#include "Core.h"

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

    POINT cursorScreen = GET_MOUSE_SCREEN_POS;
    
    int targetScreenX = cursorScreen.x - (size.cx / 2);
    int targetScreenY = cursorScreen.y - (size.cy / 2);
    
    ::SetWindowPos(
        subWindow,
        nullptr,
        targetScreenX, targetScreenY,
        0, 0,
        SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER
    );
}