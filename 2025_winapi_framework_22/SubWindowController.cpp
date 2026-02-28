#include "pch.h"
#include "SubWindowController.h"
#include "InputManager.h"
#include "Core.h"

SubWindowController::SubWindowController(HWND subWindow, SIZE windowSize)
    : m_subWindow(subWindow), m_size(windowSize)
{
}

void SubWindowController::Update()
{
    if (m_subWindow == nullptr)
        return;

    POINT cursorScreen = GET_MOUSE_SCREEN_POS;
    
    int targetScreenX = cursorScreen.x - (m_size.cx / 2);
    int targetScreenY = cursorScreen.y - (m_size.cy / 2);
    
    ::SetWindowPos(
        m_subWindow,
        nullptr,
        targetScreenX, targetScreenY,
        0, 0,
        SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER
    );
}