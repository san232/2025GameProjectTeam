#include "pch.h"
#include "SubWindowController.h"


SubWindowController::SubWindowController(HWND subWindow, SIZE windowSize)
	: subWindow(subWindow), size(windowSize) {
}


void SubWindowController::Update() {
	POINT cursor;
	GetCursorPos(&cursor);


	HWND parent = GetParent(subWindow);
	RECT parentRect;
	GetClientRect(parent, &parentRect);


	ScreenToClient(parent, &cursor);


	int x = cursor.x - size.cx / 2;
	int y = cursor.y - size.cy / 2;

	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x + size.cx > parentRect.right) x = parentRect.right - size.cx;
	if (y + size.cy > parentRect.bottom) y = parentRect.bottom - size.cy;


	SetWindowPos(subWindow, HWND_TOP, x, y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
}