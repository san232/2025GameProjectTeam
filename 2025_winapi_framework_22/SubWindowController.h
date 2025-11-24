#pragma once
#include <Windows.h>


class SubWindowController {
public:
	SubWindowController(HWND subWindow, SIZE windowSize);
	void Update();


private:
	HWND subWindow;
	SIZE size;
};