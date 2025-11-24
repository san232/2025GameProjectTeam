#include "pch.h"
#include "SubWindowRenderer.h"
#include "Scene.h"
#include "Object.h"

SubWindowRenderer::SubWindowRenderer(HWND mainWindow, Scene* scene)
	: mainWindow(mainWindow), scene(scene) {
}


void SubWindowRenderer::Render(HDC hdc, const POINT& globalPos) {
	HDC mainDC = GetDC(mainWindow);
	POINT clientPos = globalPos;
	ScreenToClient(mainWindow, &clientPos);


	BitBlt(hdc, 0, 0, 160, 160, mainDC, clientPos.x - 80, clientPos.y - 80, SRCCOPY);
	ReleaseDC(mainWindow, mainDC);


	if (scene != nullptr) {
		const auto& objects = scene->GetLayerObjects(Layer::INVISIBLEENEMY);
		for (auto obj : objects) {
			obj->Render(hdc);
		}
	}
}