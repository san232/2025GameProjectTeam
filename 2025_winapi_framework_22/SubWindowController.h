#pragma once


class SubWindowController {
public:
	SubWindowController(HWND subWindow, SIZE windowSize);
	void Update();

private:
	HWND subWindow;
	SIZE size;
};