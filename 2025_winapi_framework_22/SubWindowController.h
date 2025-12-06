#pragma once


class SubWindowController {
public:
	SubWindowController(HWND subWindow, SIZE windowSize);
	void Update();

	void ToggleMovement() { m_isMoving = !m_isMoving; }
	bool IsMoving() const { return m_isMoving; }

private:
	HWND subWindow;
	SIZE size;
	bool m_isMoving;
};